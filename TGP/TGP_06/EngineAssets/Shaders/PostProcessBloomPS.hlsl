#include "PostprocessStructs.hlsli"
#include "PostProcessBuffer.hlsli"

PostProcessPixelOutput main(PostProcessVertexToPixel input)
{
    PostProcessPixelOutput returnValue;

    float3 bloom = FullscreenTexture1.Sample(DefaultSampler, input.uv).rgb;

    returnValue.color.rgb = bloom;
    returnValue.color.a = PP_BloomStrength;
    return returnValue;
}
