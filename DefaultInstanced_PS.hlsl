#include "LightHelper.hlsli"

struct VSout
{
    float4 PosH : SV_Position;
    float3 PosW : Position;
    float3 NormalW : Normal;
    float3 tangentW : TANGENT;
    float2 Tex : TEXCOORD0;
    float4 shadowPosH : TEXCOORD1;
    float4 SSAOPosH : TEXCOORD2;

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
    bool disableTexSampling;
    bool useSSAO;
    int pad0;
    int pad1;
}


Texture2D SRVTexture : register(t0);
Texture2D SRVNormalMap : register(t1);
Texture2D SRVshadowMap : register(t2);
Texture2D SSAOAmbientMap : register(t4);
TextureCube cubeMap : register(t3);
SamplerState tex0Sample : register(s0);
SamplerComparisonState shadowSampler : register(s1);
SamplerState ssaoSampler : register(s2);


float4 main(VSout pin) : SV_TARGET
{
    // Fetch the material data.
    float4 diffuseAlbedo = mat.diffuseAlbedo;
    float3 fresnelR0 = mat.fresnelR0;
    float roughness = 1.0f - mat.shininess;
	
	// Interpolating normal can unnormalize it, so renormalize it.
    pin.NormalW = normalize(pin.NormalW);
    float4 normalMapSample = SRVNormalMap.Sample(tex0Sample, pin.Tex);

    // Vector from point being lit to eye. 
    float3 toEyeW = normalize(camPositon - pin.PosW);

    float3 bumpedNormalW = NormalSampleToWorldSpace(normalMapSample.rgb, pin.NormalW, pin.tangentW);
    
	// Dynamically look up the texture in the array.
    diffuseAlbedo *= SRVTexture.Sample(tex0Sample, pin.Tex);
    float shininess = (mat.shininess) * normalMapSample.a;

    if (disableTexSampling)
    {
        diffuseAlbedo = mat.diffuseAlbedo;
        bumpedNormalW = pin.NormalW;
        shininess = mat.shininess;
    }
    
        // Finish texture projection and sample SSAO map.
    float ambientAccess = 1.0f;
    if (useSSAO)
    {
        pin.SSAOPosH /= pin.SSAOPosH.w;
        ambientAccess = SSAOAmbientMap.Sample(ssaoSampler, pin.SSAOPosH.xy, 0.0f).r;
    }
    
    // Light terms.
    float4 ambient = ambientLight * diffuseAlbedo * ambientAccess;

    MaterialEx mat = { diffuseAlbedo, fresnelR0, shininess };
    float shadowFactor = CalcShadowFactor(shadowSampler, SRVshadowMap, pin.shadowPosH);

    DirectionalLightEx dr = dirLight;
    dr.direction = lightDirection;
    
    
    float4 result = float4(shadowFactor * ComputeDirectionalLightEx(dr, mat, bumpedNormalW, toEyeW), 0.0f);
 
    float4 litColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    litColor = ambient + result;
    
	// Add in specular reflections.
   
    float3 r = reflect(-toEyeW, bumpedNormalW);
    float4 reflectionColor = cubeMap.Sample(tex0Sample, r);
    float3 fresnelFactor = SchlickFresnel(fresnelR0, bumpedNormalW, r);
    litColor.rgb += shininess * fresnelFactor * reflectionColor.rgb;
    // Common convention to take alpha from diffuse albedo.
    litColor.a = diffuseAlbedo.a;

    return litColor;
    
    
     
   
}