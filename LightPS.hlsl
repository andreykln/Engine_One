#include "LightHelper.hlsli"

struct VertexOut
{
    float4 posH : SV_Position;
    float3 posW : POSITION;
    float3 NormalW : Normal;
    float2 Tex : TEXCOORD0;
};


cbuffer cbDefaultPS : register(b0)
{
    DirectionalLightEx dirLight;
    MaterialEx mat;
    float4 fogColor;
    float4 ambientLight;
    float3 lightDirection;
    float fogstart;
    float3 camPositon;
    float fogRange;
    bool enableNormalMapping;
}

Texture2D SRVTexture : register(t0);
SamplerState tex0Sample : register(s0);


float4 main(VertexOut pin) : SV_TARGET
{
    // Fetch the material data.
    float4 diffuseAlbedo = mat.diffuseAlbedo;
    float3 fresnelR0 = mat.fresnelR0;
    float roughness = 1.0f - mat.shininess;
	
	// Interpolating normal can unnormalize it, so renormalize it.
    pin.NormalW = normalize(pin.NormalW);
    float3 bumpedNormalW = pin.NormalW;
    float4 litColor = SRVTexture.Sample(tex0Sample, pin.Tex);
        // Vector from point being lit to eye. 
    float3 toEyeW = normalize(camPositon - pin.posW);


    bumpedNormalW = pin.NormalW;

    
	// Dynamically look up the texture in the array.
    diffuseAlbedo *= SRVTexture.Sample(tex0Sample, pin.Tex);


    // Light terms.
    float4 ambient = ambientLight * diffuseAlbedo;

    const float shininess = (mat.shininess);
    MaterialEx mat = { diffuseAlbedo, fresnelR0, shininess };
    DirectionalLightEx dr = dirLight;
    dr.direction = lightDirection;
    
    float4 result = float4(ComputeDirectionalLightEx(dr, mat, bumpedNormalW, camPositon), 0.0f);
 

    litColor = ambient + result;
    
    // Common convention to take alpha from diffuse albedo.
    litColor.a = diffuseAlbedo.a;

    return litColor;
    
    
     
   
}