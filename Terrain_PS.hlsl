#include "LightHelper.hlsli"
struct DS_OUTPUT
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float2 tex : TEXCOORD0;
    float2 tiledTex : TEXCOORD1;
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

cbuffer PS_Terrain : register(b2)
{
    float texelCellSpaceU;
    float texelCellSpaceV;
    float worldCellSpace;
}

Texture2DArray LayerMapSRV : register(t0);
Texture2D heightMap : register(t1);
Texture2D blendMap : register(t2);
Texture2D snowLayer : register(t3);
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

    //estimate normal and tangent using central differences
    float2 leftTex = pin.tex + float2(-texelCellSpaceU, 0.0f);
    float2 rightTex = pin.tex + float2(texelCellSpaceU, 0.0f);
    float2 bottomTex = pin.tex + float2(0.0f, texelCellSpaceV);
    float2 topTex = pin.tex + float2(0.0f, -texelCellSpaceV);
    
    float leftY = heightMap.SampleLevel(smpLinearWrap, leftTex, 0).r;
    float rightY = heightMap.SampleLevel(smpLinearWrap, rightTex, 0).r;
    float bottomY = heightMap.SampleLevel(smpLinearWrap, bottomTex, 0).r;
    float topY = heightMap.SampleLevel(smpLinearWrap, topTex, 0).r;
    
    float3 tangent = normalize(float3(2.0f * worldCellSpace, rightY - leftY, 0.0f));
    float3 bitan = normalize(float3(0.0f, bottomY - topY, -2.0f * worldCellSpace));
    float3 normalW = cross(tangent, bitan);
    
    //sample all layers
    float4 c0 = LayerMapSRV.Sample(smpLinearWrap, float3(pin.tiledTex, 0.0f));
    float4 c1 = LayerMapSRV.Sample(smpLinearWrap, float3(pin.tiledTex, 1.0f));
    float4 c2 = LayerMapSRV.Sample(smpLinearWrap, float3(pin.tiledTex, 2.0f));
    float4 c3 = LayerMapSRV.Sample(smpLinearWrap, float3(pin.tiledTex, 3.0f));
    float4 c4 = snowLayer.Sample(smpLinearWrap, float2(pin.tiledTex));
    
    float4 t = blendMap.Sample(smpLinearWrap, pin.tex);
    
    //blend layers on top of each other
    float4 texColor = c0;
    texColor = lerp(texColor, c1, t.r);
    texColor = lerp(texColor, c2, t.g);
    texColor = lerp(texColor, c3, t.b);
    texColor = lerp(texColor, c4, t.a);
    
    diffuseAlbedo *= texColor;
    float shininess = (mat.shininess);

       // Finish texture projection and sample SSAO map.
    float ambientAccess = 1.0f;
    
    // Light terms.
    float4 ambient = ambientLight * diffuseAlbedo * ambientAccess;

    MaterialEx mat = { diffuseAlbedo, fresnelR0, shininess };
    //float shadowFactor = CalcShadowFactor(smpShadowMap, SRVshadowMap, pin.shadowPosH);
    float shadowFactor = 1.0f;
    

    DirectionalLightEx dr = dirLight;
    dr.direction = lightDirection;
    
    
    float4 result = float4(shadowFactor * ComputeDirectionalLightEx(dr, mat, normalW, toEyeW), 0.0f);
 
    float4 litColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    litColor = ambient + result;
    
	// Add in specular reflections.
    float3 r = reflect(-toEyeW, normalW);
    float4 reflectionColor = cubeMap.Sample(smpAnisotropicWrap, r);
    float3 fresnelFactor = SchlickFresnel(fresnelR0, normalW, r);
    
    litColor.rgb += shininess * fresnelFactor * reflectionColor.rgb;
    // Common convention to take alpha from diffuse albedo.
    litColor.a = diffuseAlbedo.a;
    return litColor;
}