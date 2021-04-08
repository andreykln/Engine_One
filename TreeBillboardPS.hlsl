#include "LightHelper.hlsli"

cbuffer CB_PS_DirectionalL_Fog : register(b0)
{
    DirectionalLight dirLight[3];
    Material mat;
    float4 fogColor;
    float fogStart;
    float fogRange;
};

cbuffer CB_PS_PerFrame : register(b1)
{
    float3 cameraPositon;
    uint numberOflights;
   
}
struct GSOutput
{
    float4 posH : SV_Position;
    float3 posW : Position0;
    float3 normalW : Normal;
    float2 tex : TEXCOORD;
    uint PrimID : SV_PrimitiveID;
};

Texture2DArray SRVTexture : register(t0);
SamplerState tex0Sample : register(s0);

float4 main(GSOutput pin) : SV_TARGET
{
     // Interpolating normal can unnormalize it
    pin.normalW = normalize(pin.normalW);
    
    // The toEye vector is used in lighting.
    float3 toEye = cameraPositon - pin.posW;
    
    // Cache the distance to the eye from this surface point.
    float distToEye = length(toEye);
    
    //normalize
    toEye /= distToEye;

    float4 texColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float3 uvw = float3(pin.tex, pin.PrimID % 4);
    texColor = SRVTexture.Sample(tex0Sample, uvw);
    
    clip(texColor.a - 0.05f); 

  
    float4 litColor = texColor;

    if (numberOflights > 0)
    {
    
        float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
        [unroll]
        for (uint i = 0; i < numberOflights; ++i)
        {
            float4 A, D, S;
            ComputeDirectionalLight(mat, dirLight[i], pin.normalW, toEye, A, D, S);
            ambient += A;
            diffuse += D;
            specular += S;
        }
    
    
        litColor = texColor * (ambient + diffuse) + specular;
    }

    //fogging
    float fogLerp = saturate((distToEye - fogStart) / fogRange);
    litColor = lerp(litColor, fogColor, fogLerp);
    // Common to take alpha from diffuse material and texture
    litColor.a = mat.diffuse.a * texColor.a;

    return litColor;
    
    
    
}