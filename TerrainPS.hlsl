#include "LightHelper.hlsli"

struct DS_OUTPUT
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float2 tex : TEXCOORD0;
    float2 tiledTex : TEXCOORD1;
};

Texture2DArray LayerMapSRV : register(t0);
Texture2D heightMap : register(t1);
Texture2D blendMap : register(t2);
//TextureCube cubeMap : register(t3);

SamplerState samplerLinear : register(s0);

cbuffer CBPSDirectionalLight_Fog : register(b0)
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

cbuffer PS_Terrain : register(b2)
{
    float texelCellSpaceU;
    float texelCellSpaceV;
    float worldCellSpace;
}

float4 main(DS_OUTPUT pin) : SV_TARGET
{
    //estimate normal and tangent using central differences
    float2 leftTex = pin.tex + float2(-texelCellSpaceU, 0.0f);
    float2 rightTex = pin.tex + float2(texelCellSpaceU, 0.0f);
    float2 bottomTex = pin.tex + float2(texelCellSpaceV, 0.0f);
    float2 topTex = pin.tex + float2(-texelCellSpaceV, 0.0f);
    
    float leftY = heightMap.SampleLevel(samplerLinear, leftTex, 0).r;
    float rightY = heightMap.SampleLevel(samplerLinear, rightTex, 0).r;
    float bottomY = heightMap.SampleLevel(samplerLinear, bottomTex, 0).r;
    float topY = heightMap.SampleLevel(samplerLinear, topTex, 0).r;
    
    float3 tangent = normalize(float3(2.0f * worldCellSpace, rightY - leftY, 0.0f));
    float3 bitan = normalize(float3(0.0f, bottomY - topY, -2.0f * worldCellSpace));
    float3 normalW = cross(tangent, bitan);
    
    
    // The toEye vector is used in lighting.
    float3 toEye = camPositon - pin.posW;
    
    // Cache the distance to the eye from this surface point.
    float distToEye = length(toEye);
    
    //normalize
    toEye /= distToEye;

    //sample all layers
    float4 c0 = LayerMapSRV.Sample(samplerLinear, float3(pin.tiledTex, 0.0f));
    float4 c1 = LayerMapSRV.Sample(samplerLinear, float3(pin.tiledTex, 1.0f));
    float4 c2 = LayerMapSRV.Sample(samplerLinear, float3(pin.tiledTex, 2.0f));
    float4 c3 = LayerMapSRV.Sample(samplerLinear, float3(pin.tiledTex, 3.0f));
    float4 c4 = LayerMapSRV.Sample(samplerLinear, float3(pin.tiledTex, 4.0f));
    
    float4 t = blendMap.Sample(samplerLinear, pin.tex);
    
    //blend layers on top of each other
    float4 texColor = c0;
    texColor = lerp(texColor, c1, t.r);
    texColor = lerp(texColor, c2, t.g);
    texColor = lerp(texColor, c3, t.b);
    texColor = lerp(texColor, c4, t.a);
    
    
  
    float4 litColor = texColor;
    clip(texColor.a - 0.1f);
    
    //normal mapping
    /*float3 normalMapSample0 = normalMap0.Sample(samplerLinear, pin.texCoord).rgb;
    float3 bumpedNormalW0 = NormalSampleToWorldSpace(normalMapSample0, pin.normalW, pin.tangentW);

    float3 normalMapSample1 = normalMap1.Sample(samplerLinear, pin.texCoord).rgb;
    float3 bumpedNormalW1 = NormalSampleToWorldSpace(normalMapSample1, pin.normalW, pin.tangentW);

    float3 bumpedNormalW = normalize(bumpedNormalW0 + bumpedNormalW1);*/
    
    
    
    if (numberOfLights > 0)
    {
    
        float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
        [unroll]
        for (uint i = 0; i < numberOfLights; ++i)
        {
            float4 A, D, S;
            ComputeDirectionalLight(objectMaterial, directLight[i], normalW, toEye, A, D, S);
            ambient += A;
            diffuse += D;
            specular += S;
        }
    
    
        litColor = texColor * (ambient + diffuse) + specular;
        
       /* float3 incident = -toEye;
     //   float3 refractionVector = refract(incident, pin.NormalW, 0.97f);
        float3 reflectionVector = reflect(incident, pin.normalW);
        float4 reflectionColor = cubeMap.Sample(samplerLinear, reflectionVector);
       // float4 reflectionColor = cubeMap.Sample(tex0Sample, refractionVector);*/

       /* litColor += objectMaterial.reflect * reflectionColor;*/

    }

     //fogging
    float fogLerp = saturate((distToEye - fogStart) / fogRange);
    litColor = lerp(litColor, fogColor, fogLerp);
        // Common to take alpha from diffuse material and texture
    litColor.a = objectMaterial.diffuse.a * texColor.a;

    return litColor;
}