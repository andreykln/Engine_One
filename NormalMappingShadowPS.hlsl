#include "LightHelper.hlsli"

struct VSout
{
    float4 PosH : SV_Position;
    float3 PosW : Position;
    float3 NormalW : Normal;
    float2 Tex : TEXCOORD0;
    float3 tangentW : TANGENT;
    float4 shadowPosH : TEXCOORD1;
};

cbuffer CBPSDirectionalLight_Fog : register(b0)
{
    DirectionalLight directLight[3];
    Material objectMaterial;
    float4 fogColor;
    float fogStart;
    float fogRange;
    float2 padding;
};

cbuffer PS_Per_FrameShadowMap : register(b1)
{
    float3 lightDirection;
    unsigned int numberOfLights;
    float3 camPositon;

}

Texture2D SRVTexture : register(t0);
Texture2D SRVNormalMap : register(t1);
Texture2D SRVshadowMap : register(t2);
SamplerState tex0Sample : register(s0);
SamplerComparisonState shadowSampler : register(s1);


float4 main(VSout pin) : SV_TARGET
{
    //update rotating main light for shadow map
    //directLight[0].direction = lightDirection;
   // Interpolating normal can unnormalize it
    pin.NormalW = normalize(pin.NormalW);
    
    // The toEye vector is used in lighting.
    float3 toEye = camPositon - pin.PosW;
    
    // Cache the distance to the eye from this surface point.
    float distToEye = length(toEye);
    
    //normalize
    toEye /= distToEye;

    float4 texColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    texColor = SRVTexture.Sample(tex0Sample, pin.Tex);
  
    float4 litColor = texColor;
    clip(texColor.a - 0.1f);
    
    //normal mapping
    float3 normalMapSample = SRVNormalMap.Sample(tex0Sample, pin.Tex).rgb;
    float3 bumpedNormalW = NormalSampleToWorldSpace(normalMapSample, pin.NormalW, pin.tangentW);

    
    
    
    if (numberOfLights > 0)
    {
    
        float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
        
        float3 shadow = float3(1.0f, 1.0f, 1.0f);
        shadow[0] = CalcShadowFactor(shadowSampler, SRVshadowMap, pin.shadowPosH);
       // shadow[0] = 0.0f;
        [unroll]
        for (uint i = 0; i < numberOfLights; ++i)
        {
            float4 A, D, S;
            ComputeDirectionalLight(objectMaterial, directLight[i], bumpedNormalW, toEye, A, D, S);
            ambient += A;
            diffuse += D * shadow[0];
            specular += S * shadow[0];
        }
    
    
        litColor = texColor * (ambient + diffuse) + specular;
    }

     //fogging
    float fogLerp = saturate((distToEye - fogStart) / fogRange);
    litColor = lerp(litColor, fogColor, fogLerp);
        // Common to take alpha from diffuse material and texture
    litColor.a = objectMaterial.diffuse.a * texColor.a;

    return litColor;
}