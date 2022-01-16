#include "LightHelper.hlsli"
struct DS_OUTPUT
{
    float4 posH : SV_Position;
    float3 posW : Position;
    float3 normalW : Normal;
    float3 tangentW : TANGENT;
    float2 texCoord : TEXCOORD0;
    float2 waveDisplacementTex0 : TEXCOORD1;
    float2 waveDisplacementTex1 : TEXCOORD2;
    float2 waveNormalTex0 : TEXCOORD3;
    float2 waveNormalTex1 : TEXCOORD4;
};

cbuffer cbDefaultLightPSdata : register(b0)
{
    DirectionalLightEx dirLight;
    float4 fogColor;
    float4 ambientLight;
    float fogstart;
    float fogRange;
};
cbuffer cbDefaultLightPSPerFrame : register(b1)
{
    MaterialEx mat;
    float3 camPositon;
    bool disableTexSampling;
    float3 lightDirection;
    bool useSSAO;
};


Texture2D diffuseMap : register(t0);
Texture2D normalMap0 : register(t1);
Texture2D normalMap1 : register(t2);
//maybe add later?
//Texture2D SRVshadowMap : register(t2);
TextureCube cubeMap : register(t4);

//random vector map
SamplerState smpLinearWrap : register(s0);
SamplerState smpAnisotropicWrap : register(s1);
SamplerComparisonState smpShadowMap : register(s2);
//only for normal depth map
SamplerState smpNormalMap : register(s3);
//blur map
SamplerState smpLinearClamp : register(s4);

float4 main(DS_OUTPUT pin) : SV_TARGET
{
    // Fetch the material data.
    float4 diffuseAlbedo = mat.diffuseAlbedo;
    float3 fresnelR0 = mat.fresnelR0;
    float roughness = 1.0f - mat.shininess;

    // Vector from point being lit to eye. 
    float3 toEyeW = normalize(camPositon - pin.posW);

    float3 normalMapSample0 = normalMap0.Sample(smpAnisotropicWrap, pin.texCoord).rgb;
    float3 bumpedNormalW0 = NormalSampleToWorldSpace(normalMapSample0, pin.normalW, pin.tangentW);

    float3 normalMapSample1 = normalMap1.Sample(smpAnisotropicWrap, pin.texCoord).rgb;
    float3 bumpedNormalW1 = NormalSampleToWorldSpace(normalMapSample1, pin.normalW, pin.tangentW);

    float3 bumpedNormalW = normalize(bumpedNormalW0 + bumpedNormalW1);
    
	// Dynamically look up the texture in the array.
    diffuseAlbedo *= diffuseMap.Sample(smpAnisotropicWrap, pin.texCoord);
    float shininess = (mat.shininess);

    // Finish texture projection and sample SSAO map.
    float ambientAccess = 1.0f;
    
    // Light terms.
    float4 ambient = ambientLight * diffuseAlbedo * ambientAccess;

    MaterialEx mat = { diffuseAlbedo, fresnelR0, shininess };
    //float shadowFactor = CalcShadowFactor(smpShadowMap, SRVshadowMap, pin.shadowPosH);
    float shadowFactor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    

    DirectionalLightEx dr = dirLight;
    dr.direction = lightDirection;
    
    
    float4 result = float4(shadowFactor * ComputeDirectionalLightEx(dr, mat, bumpedNormalW, toEyeW), 0.0f);
 
    float4 litColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    litColor = ambient + result;
    
	// Add in specular reflections.
    float3 r = reflect(-toEyeW, bumpedNormalW);
    float4 reflectionColor = cubeMap.Sample(smpAnisotropicWrap, r);
    float3 fresnelFactor = SchlickFresnel(fresnelR0, bumpedNormalW, r);
    
    litColor.rgb += shininess * fresnelFactor * reflectionColor.rgb;
    // Common convention to take alpha from diffuse albedo.
    litColor.a = diffuseAlbedo.a;
    return litColor;
}