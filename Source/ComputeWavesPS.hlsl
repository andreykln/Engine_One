#include "LightHelper.hlsli"

struct PSstruct
{
    float4 PosH : SV_Position;
    float3 PosW : Position;
    float3 NormalW : Normal;
    float2 Tex : TEXCOORD;
};

Texture2D SRVTexture : register(t0);
TextureCube cubeMap : register(t4);

//random vector map
SamplerState smpLinearWrap : register(s0);
SamplerState smpAnisotropicWrap : register(s1);
SamplerComparisonState smpShadowMap : register(s2);
//only for normal depth map
SamplerState smpNormalMap : register(s3);
//blur map
SamplerState smpLinearClamp : register(s4);

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

float4 main(PSstruct pin) : SV_TARGET
{
    // Fetch the material data.
    float4 diffuseAlbedo = mat.diffuseAlbedo;
    float3 fresnelR0 = mat.fresnelR0;
    float roughness = 1.0f - mat.shininess;
	
	// Interpolating normal can unnormalize it, so renormalize it.
    pin.NormalW = normalize(pin.NormalW);
    float4 normalMapSample = float4(0.0f, 1.0f, 0.0, 1.0f);

    // Vector from point being lit to eye. 
    float3 toEyeW = normalize(camPositon - pin.PosW);

    float3 bumpedNormalW = normalMapSample.xyz;
    
	// Dynamically look up the texture in the array.
    diffuseAlbedo *= SRVTexture.Sample(smpAnisotropicWrap, pin.Tex);
    float shininess = (mat.shininess) * normalMapSample.a;

    if (disableTexSampling)
    {
        diffuseAlbedo = mat.diffuseAlbedo;
        bumpedNormalW = pin.NormalW;
        shininess = mat.shininess;
    }
    
    // Finish texture projection and sample SSAO map.
    float ambientAccess = 1.0f;

    
    // Light terms.
    float4 ambient = ambientLight * diffuseAlbedo * ambientAccess;

    MaterialEx mat = { diffuseAlbedo, fresnelR0, shininess };
    float shadowFactor = 1.0f;

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