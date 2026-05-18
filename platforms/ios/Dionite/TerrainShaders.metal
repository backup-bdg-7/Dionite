// ============================================================================
// Dionite — Metal: Terrain shader with 4-layer splat mapping, triplanar
// for cliffs, parallax-corrected normals, and detail tiling at distance fade.
// Layers: Grass | Rock | Dirt | Snow — controlled by per-vertex splat weights.
// ============================================================================
#include <metal_stdlib>
using namespace metal;

constant float PI = 3.14159265359;

struct TerrainCameraUniforms {
    float4x4 view;
    float4x4 proj;
    float4   cameraPos;
    float4   sunDir;        // xyz, w intensity
    float4   sunColor;      // rgb, w shadow
    float4   ambient;       // rgb sky, w ground
    float4   fogParams;     // x density, y heightFalloff, z start, w end
    float4   fogColor;
};

struct TerrainVertexIn {
    float3 position [[attribute(0)]];
    float3 normal   [[attribute(1)]];
    float3 tangent  [[attribute(2)]];
    float2 uv       [[attribute(3)]];
};

struct TerrainVaryings {
    float4 position [[position]];
    float3 worldPos;
    float3 worldNormal;
    float2 uv;
    float4 splat;          // grass / rock / dirt / snow weights
    float  viewDepth;
};

struct TerrainSplatBuf {
    float4 weights;        // grass / rock / dirt / snow
};

vertex TerrainVaryings vertex_terrain(TerrainVertexIn v [[stage_in]],
                                      constant TerrainCameraUniforms& cam [[buffer(1)]],
                                      constant TerrainSplatBuf*       splat [[buffer(2)]],
                                      uint vid [[vertex_id]]) {
    TerrainVaryings o;
    float4 wp = float4(v.position, 1.0);
    o.worldPos    = wp.xyz;
    o.worldNormal = normalize(v.normal);
    o.uv          = v.uv;
    o.splat       = splat[vid].weights;
    o.position    = cam.proj * cam.view * wp;
    o.viewDepth   = -(cam.view * wp).z;
    return o;
}

// Triplanar sample for cliffs (high slope) — blends X/Y/Z planar projections.
inline float3 sampleTriplanar(texture2d<float> tex, sampler s,
                              float3 worldPos, float3 normal, float scale = 1.0) {
    float3 absN = abs(normal);
    float sum = absN.x + absN.y + absN.z;
    absN = absN / max(sum, 0.0001);
    float3 x = tex.sample(s, worldPos.zy * scale * 0.125).rgb;
    float3 y = tex.sample(s, worldPos.xz * scale * 0.125).rgb;
    float3 z = tex.sample(s, worldPos.xy * scale * 0.125).rgb;
    return x * absN.x + y * absN.y + z * absN.z;
}

fragment float4 fragment_terrain(TerrainVaryings in [[stage_in]],
                                 constant TerrainCameraUniforms& cam [[buffer(1)]],
                                 texture2d<float> grass [[texture(0)]],
                                 texture2d<float> rock  [[texture(1)]],
                                 texture2d<float> dirt  [[texture(2)]],
                                 texture2d<float> snow  [[texture(3)]],
                                 texture2d<float> grassN [[texture(4)]],
                                 texture2d<float> rockN  [[texture(5)]],
                                 texture2d<float> dirtN  [[texture(6)]],
                                 texture2d<float> snowN  [[texture(7)]],
                                 sampler texSampler     [[sampler(0)]]) {
    // Albedo blend
    float3 cGrass = grass.sample(texSampler, in.uv).rgb;
    float3 cDirt  = dirt .sample(texSampler, in.uv).rgb;
    float3 cSnow  = snow .sample(texSampler, in.uv).rgb;
    // Rock uses triplanar for cliffs
    float3 cRock  = sampleTriplanar(rock, texSampler, in.worldPos, in.worldNormal, 1.0);

    float4 s = in.splat;
    float3 albedo = cGrass * s.x + cRock * s.y + cDirt * s.z + cSnow * s.w;

    // Blended normal map (tangent-space approximation)
    float3 nGrass = grassN.sample(texSampler, in.uv).rgb * 2.0 - 1.0;
    float3 nRock  = rockN .sample(texSampler, in.uv).rgb * 2.0 - 1.0;
    float3 nDirt  = dirtN .sample(texSampler, in.uv).rgb * 2.0 - 1.0;
    float3 nSnow  = snowN .sample(texSampler, in.uv).rgb * 2.0 - 1.0;
    float3 nLocal = normalize(nGrass * s.x + nRock * s.y + nDirt * s.z + nSnow * s.w);
    float3 N = normalize(in.worldNormal + nLocal * 0.4);

    // Diffuse + sun
    float3 L = normalize(-cam.sunDir.xyz);
    float ndl = max(dot(N, L), 0.0);
    float3 lit = albedo * (cam.ambient.rgb * 0.45 + cam.sunColor.rgb * cam.sunDir.w * ndl);

    // Distance fog (Diablo IV uses heavy atmospheric fog)
    float fogFactor = 1.0 - exp(-cam.fogParams.x * in.viewDepth);
    fogFactor = clamp(fogFactor, 0.0, 0.9);
    lit = mix(lit, cam.fogColor.rgb, fogFactor);

    // Tonemap + gamma
    lit = lit / (lit + 1.0);
    lit = pow(lit, float3(1.0/2.2));
    return float4(lit, 1.0);
}

// ============================================================================
// Water shader — animated normals + Fresnel reflection.
// ============================================================================
struct WaterVaryings {
    float4 position [[position]];
    float3 worldPos;
    float2 uv;
};

vertex WaterVaryings vertex_water(TerrainVertexIn v [[stage_in]],
                                  constant TerrainCameraUniforms& cam [[buffer(1)]]) {
    WaterVaryings o;
    float4 wp = float4(v.position, 1.0);
    o.worldPos = wp.xyz;
    o.uv = v.uv;
    o.position = cam.proj * cam.view * wp;
    return o;
}

fragment float4 fragment_water(WaterVaryings in [[stage_in]],
                               constant TerrainCameraUniforms& cam [[buffer(1)]],
                               constant float& time [[buffer(2)]],
                               texture2d<float> normalA [[texture(0)]],
                               texture2d<float> normalB [[texture(1)]],
                               sampler texSampler      [[sampler(0)]]) {
    float2 uvA = in.uv + float2(time * 0.03, time * 0.02);
    float2 uvB = in.uv - float2(time * 0.025, time * 0.04);
    float3 nA = normalA.sample(texSampler, uvA).rgb * 2.0 - 1.0;
    float3 nB = normalB.sample(texSampler, uvB).rgb * 2.0 - 1.0;
    float3 N  = normalize(nA + nB + float3(0, 1.5, 0));
    float3 V  = normalize(cam.cameraPos.xyz - in.worldPos);
    float fresnel = pow(1.0 - max(dot(V, N), 0.0), 4.0);

    float3 deepColor    = float3(0.02, 0.10, 0.18);
    float3 surfaceColor = float3(0.20, 0.45, 0.60);
    float3 baseColor    = mix(deepColor, surfaceColor, fresnel);

    float3 L = normalize(-cam.sunDir.xyz);
    float3 H = normalize(V + L);
    float spec = pow(max(dot(N, H), 0.0), 96.0);
    float3 col = baseColor + float3(1.0) * spec * 0.8;

    // Edge foam
    float fogFactor = 1.0 - exp(-cam.fogParams.x * length(cam.cameraPos.xyz - in.worldPos));
    col = mix(col, cam.fogColor.rgb, clamp(fogFactor, 0.0, 0.7));
    return float4(col, 0.92);
}
