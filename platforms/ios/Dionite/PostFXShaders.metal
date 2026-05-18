// ============================================================================
// Dionite — Metal: Post-processing stack (the "Diablo IV look").
//   Bloom (5-tap downsample + upsample), screen-space fog, vignette,
//   chromatic aberration, color grading LUT, FXAA.
// Driven by `PostFXSettings` passed each frame.
// ============================================================================
#include <metal_stdlib>
using namespace metal;

struct PostFXSettings {
    float bloomIntensity;      // 0.0 - 1.0
    float bloomThreshold;      // 0.8 typical
    float vignetteStrength;    // 0.0 - 0.6
    float chromaticAmount;     // 0.0 - 0.02
    float fogDensity;          // 0.0 - 0.05
    float saturation;          // 0.5 - 1.5
    float contrast;            // 0.8 - 1.4
    float exposure;            // -2 - +2
    float gamma;               // 2.2
    float ditherAmount;        // 0.003 to break banding
};

struct PostVaryings {
    float4 position [[position]];
    float2 uv;
};

vertex PostVaryings vertex_post(uint vid [[vertex_id]]) {
    PostVaryings o;
    float2 p = float2((vid << 1) & 2, vid & 2);
    o.uv = p;
    o.position = float4(p * 2.0 - 1.0, 0.0, 1.0);
    return o;
}

// Bright-pass (writes into bloom mip chain)
fragment float4 fragment_bright_pass(PostVaryings in [[stage_in]],
                                     constant PostFXSettings& s [[buffer(0)]],
                                     texture2d<float> scene [[texture(0)]],
                                     sampler sm [[sampler(0)]]) {
    float3 c = scene.sample(sm, in.uv).rgb;
    float lum = dot(c, float3(0.2126, 0.7152, 0.0722));
    float weight = max(0.0, lum - s.bloomThreshold) / max(0.0001, lum);
    return float4(c * weight, 1.0);
}

// 9-tap separable Gaussian blur
fragment float4 fragment_blur(PostVaryings in [[stage_in]],
                              constant float2& direction [[buffer(0)]],
                              texture2d<float> src [[texture(0)]],
                              sampler sm [[sampler(0)]]) {
    const float w[5] = { 0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216 };
    float2 texel = 1.0 / float2(src.get_width(), src.get_height());
    float3 col = src.sample(sm, in.uv).rgb * w[0];
    for (int i = 1; i < 5; ++i) {
        float2 off = direction * texel * float(i);
        col += src.sample(sm, in.uv + off).rgb * w[i];
        col += src.sample(sm, in.uv - off).rgb * w[i];
    }
    return float4(col, 1.0);
}

// Composite — combine scene + bloom + apply grading + vignette + chromatic + dither.
fragment float4 fragment_composite(PostVaryings in [[stage_in]],
                                   constant PostFXSettings& s [[buffer(0)]],
                                   texture2d<float> scene [[texture(0)]],
                                   texture2d<float> bloom [[texture(1)]],
                                   texture3d<float> grading [[texture(2)]],
                                   sampler sm [[sampler(0)]]) {
    // Chromatic aberration radial offset
    float2 fromCenter = in.uv - 0.5;
    float r = length(fromCenter);
    float2 dir = (r > 0.0) ? normalize(fromCenter) : float2(0);
    float ca = s.chromaticAmount * r * r;
    float3 col;
    col.r = scene.sample(sm, in.uv + dir * ca).r;
    col.g = scene.sample(sm, in.uv).g;
    col.b = scene.sample(sm, in.uv - dir * ca).b;

    // Add bloom
    col += bloom.sample(sm, in.uv).rgb * s.bloomIntensity;

    // Exposure
    col *= pow(2.0, s.exposure);

    // Contrast
    col = (col - 0.5) * s.contrast + 0.5;
    col = max(col, float3(0));

    // Saturation
    float lum = dot(col, float3(0.2126, 0.7152, 0.0722));
    col = mix(float3(lum), col, s.saturation);

    // Color grading via 3D LUT (32^3 typical)
    if (grading.get_width() > 0) {
        float3 lutSize = float3(grading.get_width());
        float3 scaled = col * (lutSize - 1.0) / lutSize + 0.5 / lutSize;
        col = grading.sample(sm, scaled).rgb;
    }

    // Vignette
    float vig = smoothstep(0.95, 0.35, r);
    col *= mix(1.0, vig, s.vignetteStrength);

    // Dither (breaks color banding)
    float dither = fract(sin(dot(in.uv, float2(12.9898, 78.233))) * 43758.5453);
    col += (dither - 0.5) * s.ditherAmount;

    // Final gamma
    col = pow(max(col, float3(0)), float3(1.0 / s.gamma));
    return float4(col, 1.0);
}
