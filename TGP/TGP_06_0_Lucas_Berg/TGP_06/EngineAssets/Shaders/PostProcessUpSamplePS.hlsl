#include "PostprocessStructs.hlsli"
#include "PostProcessBuffer.hlsli"

PostProcessPixelOutput main(PostProcessVertexToPixel input)
{
    PostProcessPixelOutput returnValue;

    float2 pixelOffset = float2(ddx(input.uv.x), ddy(input.uv.y));

    float3 p00 = FullscreenTexture1.Sample(DefaultSampler, input.uv + pixelOffset * float2(-1.0f, -1.0f)).rgb;
    float3 p01 = FullscreenTexture1.Sample(DefaultSampler, input.uv + pixelOffset * float2(-1.0f, 1.0f)).rgb;
    float3 p10 = FullscreenTexture1.Sample(DefaultSampler, input.uv + pixelOffset * float2(1.0f, -1.0f)).rgb;
    float3 p11 = FullscreenTexture1.Sample(DefaultSampler, input.uv + pixelOffset * float2(1.0f, 1.0f)).rgb;

    returnValue.color.rgb = 0.25f * (p00 + p01 + p10 + p11);
    returnValue.color.a = PP_BloomBlending;
    return returnValue;
}
