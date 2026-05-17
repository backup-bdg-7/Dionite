// ============================================================================
// Dionite — Metal Shader (vertex + fragment for the basic PBR-lite pipeline)
// ============================================================================
#include <metal_stdlib>
using namespace metal;

struct VertexIn {
    float3 position [[attribute(0)]];
    float3 normal   [[attribute(1)]];
    float2 uv       [[attribute(2)]];
};

struct VertexOut {
    float4 position [[position]];
    float3 worldNormal;
    float2 uv;
    float3 worldPos;
};

struct CameraUniforms {
    float4x4 view;
    float4x4 proj;
    float4   lightDir;   // xyz dir, w intensity
    float4   ambient;    // rgb, w unused
    float4   cameraPos;
};

struct ModelUniforms {
    float4x4 model;
    float4   tint;
};

vertex VertexOut vertex_main(VertexIn in [[stage_in]],
                             constant CameraUniforms& cam [[buffer(1)]],
                             constant ModelUniforms&  mdl [[buffer(2)]]) {
    VertexOut out;
    float4 world = mdl.model * float4(in.position, 1.0);
    out.position = cam.proj * cam.view * world;
    out.worldNormal = normalize((mdl.model * float4(in.normal, 0)).xyz);
    out.uv = in.uv;
    out.worldPos = world.xyz;
    return out;
}

fragment float4 fragment_main(VertexOut in [[stage_in]],
                              constant CameraUniforms& cam [[buffer(1)]],
                              constant ModelUniforms&  mdl [[buffer(2)]],
                              texture2d<float> albedoTex [[texture(0)]],
                              sampler texSampler [[sampler(0)]]) {
    float4 albedo = albedoTex.sample(texSampler, in.uv) * mdl.tint;
    float ndotl = max(dot(in.worldNormal, -cam.lightDir.xyz), 0.0);
    float3 lit = albedo.rgb * (cam.ambient.rgb + ndotl * cam.lightDir.w);
    // Rim light for that fantasy glow
    float3 viewDir = normalize(cam.cameraPos.xyz - in.worldPos);
    float rim = pow(1.0 - max(dot(viewDir, in.worldNormal), 0.0), 3.0);
    lit += rim * 0.25 * float3(1.0, 0.85, 0.55);
    return float4(lit, albedo.a);
}
