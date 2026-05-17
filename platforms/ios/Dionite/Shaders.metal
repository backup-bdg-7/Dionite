// ============================================================================
// Dionite — Metal Shaders: full PBR pipeline with cascaded shadow maps,
// dynamic point lights, normal mapping, and emissive bloom feed.
// Author: Dionite Studio Starter Kit
// ============================================================================
#include <metal_stdlib>
using namespace metal;

constant float PI = 3.14159265359;

// ---------------------------------------------------------------------------
// Uniforms
// ---------------------------------------------------------------------------
struct DirectionalLight {
    float4 direction;   // xyz, w intensity
    float4 color;       // rgb, w castShadow
};

struct CameraUniforms {
    float4x4 view;
    float4x4 proj;
    float4   cameraPos;        // xyz, w unused
    DirectionalLight sun;
    float4x4 sunVP[4];
    float4   cascadeSplits;    // 4 cascade far distances
};

struct ModelUniforms {
    float4x4 model;
    float4   albedoTint;       // rgb tint, a emissive strength
    float4   matParams;        // x metallic, y roughness, z receivesShadow, w castsShadow
};

struct PointLight {
    float4 position;   // xyz, w radius
    float4 color;      // rgb, w intensity
};

// ---------------------------------------------------------------------------
// Vertex
// ---------------------------------------------------------------------------
struct VertexIn {
    float3 position [[attribute(0)]];
    float3 normal   [[attribute(1)]];
    float3 tangent  [[attribute(2)]];
    float2 uv       [[attribute(3)]];
};

struct VertexOut {
    float4 position [[position]];
    float3 worldPos;
    float3 worldNormal;
    float3 worldTangent;
    float3 worldBitangent;
    float2 uv;
};

vertex VertexOut vertex_main(VertexIn in [[stage_in]],
                             constant CameraUniforms& cam [[buffer(1)]],
                             constant ModelUniforms&  mdl [[buffer(2)]]) {
    VertexOut out;
    float4 world = mdl.model * float4(in.position, 1.0);
    out.position = cam.proj * cam.view * world;
    out.worldPos = world.xyz;
    out.worldNormal    = normalize((mdl.model * float4(in.normal, 0)).xyz);
    out.worldTangent   = normalize((mdl.model * float4(in.tangent, 0)).xyz);
    out.worldBitangent = cross(out.worldNormal, out.worldTangent);
    out.uv = in.uv;
    return out;
}

// ---------------------------------------------------------------------------
// PBR helpers (Disney/UE4 lite — GGX + Smith + Schlick)
// ---------------------------------------------------------------------------
inline float3 fresnelSchlick(float cosT, float3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosT, 5.0);
}
inline float ggx(float ndh, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float denom = (ndh * ndh) * (a2 - 1.0) + 1.0;
    return a2 / (PI * denom * denom);
}
inline float smithG(float ndv, float ndl, float roughness) {
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    float gv = ndv / (ndv * (1.0 - k) + k);
    float gl = ndl / (ndl * (1.0 - k) + k);
    return gv * gl;
}

// ---------------------------------------------------------------------------
// Shadow sampling — pick cascade by depth
// ---------------------------------------------------------------------------
inline float sampleShadow(float3 worldPos, float viewDepth,
                          constant CameraUniforms& cam,
                          depth2d_array<float> shadowMaps,
                          sampler shadowSampler) {
    int cascade = 3;
    for (int i = 0; i < 4; ++i) {
        if (viewDepth < cam.cascadeSplits[i]) { cascade = i; break; }
    }
    float4 lp = cam.sunVP[cascade] * float4(worldPos, 1.0);
    float3 ndc = lp.xyz / lp.w;
    float2 uv = ndc.xy * 0.5 + 0.5;
    if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) return 1.0;
    float bias = 0.0008;
    float lit = 0.0;
    for (int dy = -1; dy <= 1; ++dy)
      for (int dx = -1; dx <= 1; ++dx) {
        float depth = shadowMaps.sample(shadowSampler, uv + float2(dx, dy) * (1.0/2048.0), cascade);
        lit += (ndc.z - bias < depth) ? 1.0 : 0.0;
      }
    return lit / 9.0;
}

// ---------------------------------------------------------------------------
// Fragment
// ---------------------------------------------------------------------------
fragment float4 fragment_main(VertexOut in [[stage_in]],
                              constant CameraUniforms& cam [[buffer(1)]],
                              constant ModelUniforms&  mdl [[buffer(2)]],
                              constant PointLight* pointLights [[buffer(3)]],
                              constant uint& pointLightCount [[buffer(4)]],
                              texture2d<float> albedoTex   [[texture(0)]],
                              texture2d<float> normalTex   [[texture(1)]],
                              texture2d<float> mraTex      [[texture(2)]],
                              texture2d<float> emissiveTex [[texture(3)]],
                              depth2d_array<float> shadowMaps [[texture(4)]],
                              sampler texSampler           [[sampler(0)]],
                              sampler shadowSampler        [[sampler(1)]]) {
    // ---- Material sample ---------------------------------------------------
    float4 albedo = albedoTex.sample(texSampler, in.uv);
    albedo.rgb *= mdl.albedoTint.rgb;
    float3 mra    = mraTex.sample(texSampler, in.uv).rgb;
    float metallic = mra.r * mdl.matParams.x + mdl.matParams.x;
    float roughness = clamp(mra.g * mdl.matParams.y + 0.04, 0.04, 1.0);
    float ao        = mra.b;
    float3 emissive = emissiveTex.sample(texSampler, in.uv).rgb * mdl.albedoTint.a;

    // ---- Normal mapping ---------------------------------------------------
    float3 nMap = normalTex.sample(texSampler, in.uv).rgb * 2.0 - 1.0;
    float3x3 TBN = float3x3(normalize(in.worldTangent),
                            normalize(in.worldBitangent),
                            normalize(in.worldNormal));
    float3 N = normalize(TBN * nMap);
    float3 V = normalize(cam.cameraPos.xyz - in.worldPos);

    float3 F0 = mix(float3(0.04), albedo.rgb, metallic);
    float3 Lo = float3(0.0);

    // ---- Sun (directional) -----------------------------------------------
    {
        float3 L = normalize(-cam.sun.direction.xyz);
        float3 H = normalize(V + L);
        float ndl = max(dot(N, L), 0.0);
        float ndv = max(dot(N, V), 0.0);
        float ndh = max(dot(N, H), 0.0);
        float3 F  = fresnelSchlick(max(dot(H, V), 0.0), F0);
        float D   = ggx(ndh, roughness);
        float G   = smithG(ndv, ndl, roughness);
        float3 spec = (D * G * F) / max(4.0 * ndv * ndl, 0.001);
        float3 kd = (float3(1.0) - F) * (1.0 - metallic);
        float viewDepth = -(cam.view * float4(in.worldPos, 1.0)).z;
        float shadow = (cam.sun.color.w > 0.5)
            ? sampleShadow(in.worldPos, viewDepth, cam, shadowMaps, shadowSampler)
            : 1.0;
        Lo += (kd * albedo.rgb / PI + spec) * cam.sun.color.rgb * cam.sun.direction.w * ndl * shadow;
    }

    // ---- Point lights ----------------------------------------------------
    for (uint i = 0; i < pointLightCount; ++i) {
        float3 lp = pointLights[i].position.xyz;
        float radius = pointLights[i].position.w;
        float3 toL = lp - in.worldPos;
        float dist = length(toL);
        if (dist > radius) continue;
        float3 L = toL / max(dist, 0.0001);
        float atten = pow(1.0 - dist / radius, 2.0);
        float3 H = normalize(V + L);
        float ndl = max(dot(N, L), 0.0);
        float ndv = max(dot(N, V), 0.0);
        float ndh = max(dot(N, H), 0.0);
        float3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
        float D = ggx(ndh, roughness);
        float G = smithG(ndv, ndl, roughness);
        float3 spec = (D * G * F) / max(4.0 * ndv * ndl, 0.001);
        float3 kd = (float3(1.0) - F) * (1.0 - metallic);
        Lo += (kd * albedo.rgb / PI + spec) * pointLights[i].color.rgb * pointLights[i].color.w * ndl * atten;
    }

    // ---- Ambient + emissive + rim ----------------------------------------
    float3 ambient = float3(0.03) * albedo.rgb * ao;
    float rim = pow(1.0 - max(dot(V, N), 0.0), 3.0) * 0.18;
    float3 color = ambient + Lo + emissive + rim * float3(1.0, 0.85, 0.55);

    // ---- Reinhard tonemap + gamma -----------------------------------------
    color = color / (color + float3(1.0));
    color = pow(color, float3(1.0 / 2.2));
    return float4(color, albedo.a);
}

// ---------------------------------------------------------------------------
// Shadow-only pass — depth write to cascade slices
// ---------------------------------------------------------------------------
struct ShadowVertexOut { float4 position [[position]]; };

vertex ShadowVertexOut vertex_shadow(VertexIn in [[stage_in]],
                                     constant CameraUniforms& cam [[buffer(1)]],
                                     constant ModelUniforms&  mdl [[buffer(2)]],
                                     constant uint& cascadeIndex  [[buffer(5)]]) {
    ShadowVertexOut out;
    float4 world = mdl.model * float4(in.position, 1.0);
    out.position = cam.sunVP[cascadeIndex] * world;
    return out;
}

fragment void fragment_shadow() {}
