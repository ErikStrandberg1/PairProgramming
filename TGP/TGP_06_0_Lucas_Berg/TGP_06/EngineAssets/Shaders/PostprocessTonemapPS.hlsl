#include "PostprocessStructs.hlsli"
#include "Common.hlsli"
#include "PostProcessBuffer.hlsli"

float3 s_curve(float3 x)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

float3 tonemap_s_gamut3_cine(float3 c)
{
    float3x3 fromSrgb = float3x3(
        +0.6456794776, +0.2591145470, +0.0952059754,
        +0.0875299915, +0.7596995626, +0.1527704459,
        +0.0369574199, +0.1292809048, +0.8337616753);

    float3x3 toSrgb = float3x3(
        +1.6269474099, -0.5401385388, -0.0868088707,
        -0.1785155272, +1.4179409274, -0.2394254004,
        -0.0444361150, -0.1959199662, +1.2403560812);

    return mul(toSrgb, s_curve(mul(fromSrgb, c)));
}

PostProcessPixelOutput main(PostProcessVertexToPixel input)
{
    PostProcessPixelOutput returnValue;

    float3 color = FullscreenTexture1.Sample(DefaultSampler, input.uv).rgb;

    // Color grading (before tonemapping)
    // Saturation
    float luminance = dot(float3(0.2126f, 0.7152f, 0.0722f), color);
    color = luminance + PP_Saturation * (color - luminance);

    // Exposure
    color = exp2(PP_Exposure) * color;

    // Contrast 
    color = 0.18f * pow(max(color, 0.0001f) / 0.18f, PP_Contrast);

    // Tint
    color = color * PP_Tint;

    // Black point
    color = max(0.0f, color - PP_BlackPoint);

    //  Tonemapping 
    color = tonemap_s_gamut3_cine(color);

    returnValue.color = float4(color, 1.0f);
    return returnValue;
}
