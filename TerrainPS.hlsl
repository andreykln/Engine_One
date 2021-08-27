#include "LightHelper.hlsli"

struct DS_OUTPUT
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float2 tex : TEXCOORD0;
    float2 tiledTex : TEXCOORD1;
};

Texture2D diffuseMap : register(t0);
Texture2D normalMap0 : register(t1);
Texture2D normalMap1 : register(t2);
TextureCube cubeMap : register(t3);

SamplerState samplerLinear : register(s0);

cbuffer CBPSDirectionalLight_Fog
{
    DirectionalLight directLight[3];
    Material objectMaterial;
    float4 fogColor;
    float fogStart;
    float fogRange;
    float2 padding;
};

cbuffer PS_Per_Frame : register(b1)
{
    float3 camPositon;
    unsigned int numberOfLights;
    unsigned int texArrayPosition;
}


float4 main(DS_OUTPUT pin) : SV_TARGET
{
   // Interpolating normal can unnormalize it
    pin.normalW = normalize(pin.normalW);
    
    // The toEye vector is used in lighting.
    float3 toEye = camPositon - pin.posW;
    
    // Cache the distance to the eye from this surface point.
    float distToEye = length(toEye);
    
    //normalize
    toEye /= distToEye;

    float4 texColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    texColor = diffuseMap.Sample(samplerLinear, pin.texCoord);
  
    float4 litColor = texColor;
    clip(texColor.a - 0.1f);
    
    //normal mapping
    float3 normalMapSample0 = normalMap0.Sample(samplerLinear, pin.texCoord).rgb;
    float3 bumpedNormalW0 = NormalSampleToWorldSpace(normalMapSample0, pin.normalW, pin.tangentW);

    float3 normalMapSample1 = normalMap1.Sample(samplerLinear, pin.texCoord).rgb;
    float3 bumpedNormalW1 = NormalSampleToWorldSpace(normalMapSample1, pin.normalW, pin.tangentW);

    float3 bumpedNormalW = normalize(bumpedNormalW0 + bumpedNormalW1);
    
    
    
    if (numberOfLights > 0)
    {
    
        float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
        [unroll]
        for (uint i = 0; i < numberOfLights; ++i)
        {
            float4 A, D, S;
            ComputeDirectionalLight(objectMaterial, directLight[i], bumpedNormalW, toEye, A, D, S);
            ambient += A;
            diffuse += D;
            specular += S;
        }
    
    
        litColor = texColor * (ambient + diffuse) + specular;
        
        float3 incident = -toEye;
     //   float3 refractionVector = refract(incident, pin.NormalW, 0.97f);
        float3 reflectionVector = reflect(incident, pin.normalW);
        float4 reflectionColor = cubeMap.Sample(samplerLinear, reflectionVector);
       // float4 reflectionColor = cubeMap.Sample(tex0Sample, refractionVector);

        litColor += objectMaterial.reflect * reflectionColor;

    }

     //fogging
    float fogLerp = saturate((distToEye - fogStart) / fogRange);
    litColor = lerp(litColor, fogColor, fogLerp);
        // Common to take alpha from diffuse material and texture
    litColor.a = objectMaterial.diffuse.a * texColor.a;

    return litColor;
}