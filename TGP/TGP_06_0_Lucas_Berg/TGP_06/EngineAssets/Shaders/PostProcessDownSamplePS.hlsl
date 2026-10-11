#include "PostprocessStructs.hlsli"

PostProcessPixelOutput main(PostProcessVertexToPixel input)
{
    PostProcessPixelOutput returnValue;

    float2 pixelOffset = float2(ddx(input.uv.x), ddy(input.uv.y));

    float3 p00 = FullscreenTexture1.Sample(DefaultSampler, input.uv + pixelOffset * float2(-0.5f, -0.5f)).rgb;
    float3 p01 = FullscreenTexture1.Sample(DefaultSampler, input.uv + pixelOffset * float2(-0.5f,  0.5f)).rgb;
    float3 p10 = FullscreenTexture1.Sample(DefaultSampler, input.uv + pixelOffset * float2( 0.5f, -0.5f)).rgb;
    float3 p11 = FullscreenTexture1.Sample(DefaultSampler, input.uv + pixelOffset * float2( 0.5f,  0.5f)).rgb;

    returnValue.color.rgb = 0.25f * (p00 + p01 + p10 + p11);
    returnValue.color.a = 1.0f;
    return returnValue;
}
