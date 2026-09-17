#include "Common.hlsli"
#include "PBRFunctions.hlsli"

float EvaluateSpotShadow(float4 worldPosition, float3 worldNormal)
{
    float4 pos = worldPosition;
    pos.xyz += 1.0f * normalize(worldNormal);

    float4 lightClipPos = mul(SpotLightWorldToLightClip, pos);
    float3 projected = lightClipPos.xyz / lightClipPos.w;

    float2 shadowUV = 0.5f + float2(0.5f, -0.5f) * projected.xy;

    if (shadowUV.x < 0.f || shadowUV.x > 1.f || shadowUV.y < 0.f || shadowUV.y > 1.f)
    {
        return 1.0f;
    }

    float shadowMapDepth = spotLightShadowMap.SampleLevel(defaultSampler, shadowUV, 0).r;
    float bias = 0.0005f;

    return (projected.z > shadowMapDepth + bias) ? 0.0f : 1.0f;
}


PixelOutput main(ModelVertexToPixel input)
{
	PixelOutput result;

	float2 scaledUV = input.texCoord0;
	
	float3 toEye = normalize(CameraToWorld._m03_m13_m23 - input.worldPosition.xyz);
    float4 albedo = albedoTexture.Sample(defaultSampler, scaledUV).rgba;

	if (albedo.a <= AlphaTestThreshold)
	{
		discard;
		result.color = float4(0.f, 0.f, 0.f, 0.f);
		return result;
	}

	float3 normal = normalTexture.Sample(defaultSampler, scaledUV).xyy;

	normal.xy = 2.0f * normal.xy - 1.0f;
	normal.z = sqrt(1 - saturate(normal.x * normal.x + normal.y * normal.y));
	normal = normalize(normal);

	float3x3 TBN = float3x3(
		normalize(input.tangent.xyz),
		normalize(-input.binormal.xyz),
		normalize(input.normal.xyz)
		);

	// Can save an instruction here by instead doing
	// normalize(mul(normal, TBN)); It works because
	// TBN is a 3x3 and therefore TBN^T is the same
	// as TBN^-1. However, it is considered good form
	// to do this.
	TBN = transpose(TBN);
	float3 pixelNormal = normalize(mul(TBN, normal));

	// TGA Channel Pack. ORM.
	// Metalness, Roughness, Emissive, Emissive Strength (opt).

    float3 material = materialTexture.Sample(defaultSampler, scaledUV).rgb;

    float ambientOcclusion = material.r;
	float metalness = material.b;
    float roughness = material.g;

    float3 fx = fxTexture.Sample(defaultSampler, scaledUV).rgb;

    float emissive = fx.r;
	
	float3 specularColor = lerp((float3) 0.04f, albedo.rgb, metalness);
	float3 diffuseColor = lerp((float3) 0.00f, albedo.rgb, 1 - metalness);

    float3 ambiance = AmbientLightColor.rgb * EvaluateAmbiance(
		environmentTexture, pixelNormal, input.normal.xyz,
		toEye, roughness,
		ambientOcclusion, diffuseColor, specularColor
	);

	float3 directionalLight;


	if (DirectionalLightSoftness == 0.f)
	{
		directionalLight = EvaluateDirectionalLight(
			diffuseColor, specularColor, pixelNormal, roughness,
			DirectionalLightColor.xyz, DirectionalLightToWorldTransform._m02_m12_m22, toEye.xyz);
    }
	else 
	{
		directionalLight = EvaluateSoftDirectionalLight(
			diffuseColor, specularColor, pixelNormal, roughness, DirectionalLightSoftness,
			DirectionalLightColor.xyz, DirectionalLightToWorldTransform._m02_m12_m22, toEye.xyz);
    }

	float3 pointLights = 0; 
	for(unsigned int p = 0; p < NumberOfLights; p++)
	{
		if (PointLights[p].radius == 0.f)
		{
			pointLights += EvaluatePointLight(
				diffuseColor, specularColor, pixelNormal, roughness,
				PointLights[p].color.rgb, PointLights[p].range, PointLights[p].position.xyz,
				toEye.xyz, input.worldPosition.xyz);
		}
		else
		{
			pointLights += EvaluateSoftAreaLight(
				diffuseColor, specularColor, pixelNormal, roughness,
				PointLights[p].color.rgb, PointLights[p].radius, PointLights[p].range, PointLights[p].position.xyz,
				toEye.xyz, input.worldPosition.xyz);
		}
	}
	
    float spotShadow = EvaluateSpotShadow(input.worldPosition, input.normal.xyz);
    float3 spotLightContribution = spotShadow * EvaluateSpotLight(
		diffuseColor, specularColor, pixelNormal, roughness,
		SpotLightColor.rgb, SpotLightRange, SpotLightPosition.xyz, SpotLightDirection.xyz,
		SpotLightOuterConeAngle, SpotLightInnerConeAngle, toEye.xyz, input.worldPosition.xyz);

    float3 emissiveAlbedo = albedo.rgb * emissive;
    float3 radiance = directionalLight + ambiance + pointLights + spotLightContribution + emissiveAlbedo;

    result.color.rgb = (float3) radiance;
	result.color.a = albedo.a;
	return result;
}

