#include "LightHelper.hlsli"

cbuffer CBPSAllLight : register(b0)
{
    DirectionalLight directLight[3];
    PointLight pointLight;
    SpotLight spotLight;
    Material objectMaterial;
    float3 cameraPosition;
    uint numberOfLights;
    float4 fogColor;
    float fogStart;
    float fogRange;
    float2 padding;
};


Texture2D SRVTexture : register(t0);
SamplerState tex0Sample : register(s0);

struct PSstruct
{
    float4 PosH : SV_Position;
    float3 PosW : Position;
    float3 NormalW : Normal;
    float2 Tex : TEXCOORD;
};


float4 main(PSstruct pin) : SV_TARGET
{
    // Interpolating normal can unnormalize it
    pin.NormalW = normalize(pin.NormalW);
    
    // The toEye vector is used in lighting.
    float3 toEye = cameraPosition - pin.PosW;
    
    // Cache the distance to the eye from this surface point.
    float distToEye = length(toEye);
    
    //normalize
    toEye /= distToEye;

    float4 texColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    texColor = SRVTexture.Sample(tex0Sample, pin.Tex);
  
    float4 litColor = texColor;
    clip(texColor.a - 0.1f);
    
    if (numberOfLights > 0)
    {
    
        float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
        
        float4 A, D, S;
        [unroll]
        for (uint i = 0; i < numberOfLights; ++i)
        {
            ComputeDirectionalLight(objectMaterial, directLight[i], pin.NormalW, toEye, A, D, S);
            ambient += A;
            diffuse += D;
            specular += S;
        }
    
        ComputePointLight(objectMaterial, pointLight, pin.PosW, pin.NormalW, toEye, A, D, S);
        ambient += A;
        diffuse += D;
        specular += S;

        ComputeSpotLight(objectMaterial, spotLight, pin.PosW, pin.NormalW, toEye, A, D, S);
        ambient += A;
        diffuse += D;
        specular += S;
        litColor = texColor * (ambient + diffuse) + specular;
    }

     //fogging
    float fogLerp = saturate((distToEye - fogStart) / fogRange);
    litColor = lerp(litColor, fogColor, fogLerp);
        // Common to take alpha from diffuse material and texture
    litColor.a = objectMaterial.diffuse.a * texColor.a;

    return litColor;
}