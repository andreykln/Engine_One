#include "LightHelper.hlsli"

struct VSout
{
    float4 PosH : SV_Position;
    float3 PosW : Position;
    float3 NormalW : Normal;
    float4 shadowPosH : TEXCOORD1;
};

cbuffer CBPSDirectionalLight_Fog : register(b0)
{
    DirectionalLightEx directLightEx;
    MaterialEx objectMaterial;
    float4 ambientLight;
    float4 fogColor;
    float fogStart;
    float fogRange;
    float pad1;
    float pad2;
    
};

cbuffer PS_Per_FrameShadowMap : register(b1)
{
    float3 lightDirection;
    unsigned int numberOfLights;
    float3 camPositon;

}
TextureCube cubeMap : register(t3);
SamplerState tex0Sample : register(s0);
Texture2D SRVshadowMap : register(t2);
SamplerComparisonState shadowSampler : register(s1);


float4 main(VSout pin) : SV_TARGET
{
    
        // Interpolating normal can unnormalize it, so renormalize it.
    pin.NormalW = normalize(pin.NormalW);

    // Vector from point being lit to eye. 
    float3 toEyeW = normalize(camPositon - pin.PosW);

	// Indirect lighting.
    float4 ambient = ambientLight * objectMaterial.diffuseAlbedo;

    float3 shadowFactor = 1.0f;
    DirectionalLightEx dr = directLightEx;
    dr.direction = lightDirection;
    float4 directLight = float4(shadowFactor * ComputeDirectionalLightEx(dr, objectMaterial, pin.NormalW, toEyeW), 0.0f);

    float4 litColor = ambient + directLight;

    
    // Add in specular reflections.
    float3 r = reflect(-toEyeW, pin.NormalW);
    float4 reflectionColor = cubeMap.Sample(tex0Sample, r);
    float3 fresnelFactor = SchlickFresnel(objectMaterial.fresnelR0, pin.NormalW, r);
    litColor.rgb += objectMaterial.shininess * fresnelFactor * reflectionColor.rgb;
    
    // Common convention to take alpha from diffuse material.
    litColor.a = objectMaterial.diffuseAlbedo.a;

    return litColor;
    
}