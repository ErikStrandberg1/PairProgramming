#ifndef POSTPROCESSBUFFER_HLSLI
#define POSTPROCESSBUFFER_HLSLI

// Slots b0-b5 are used by the engine (see Common.hlsli).
cbuffer PostProcessBuffer : register(b10)
{
    float3 PP_Tint;
    float PP_Exposure;
    float3 PP_Contrast;
    float PP_Saturation;
    float3 PP_BlackPoint;
    float PP_BloomBlending;
    float PP_BloomStrength;
    float3 PP_Padding;
}

#endif
