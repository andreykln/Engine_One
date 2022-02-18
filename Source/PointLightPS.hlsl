#include "LightHelper.hlsli"
struct VertexOut
{
    float4 PosH : SV_Position;
    float3 PosW : Position;
    float3 NormalW : Normal;
    float3 tangentW : TANGENT;
    float2 Tex : TEXCOORD0;
    float4 shadowPosH : TEXCOORD1;
    float4 SSAOPosH : TEXCOORD2;
    float3 lightPos[2] : TEXCOORD3;
};


cbuffer cbDefaultLightPSPerFrame : register(b0)
{
    MaterialEx mat;
    float3 camPositon;
    bool disableTexSampling;
    float3 lightDirection;
    bool useSSAO;
    bool alphaClip;
    float randomPart;
};
cbuffer cbMultiplePointLight : register(b1)
{
    float4 lightPos[2];
    float4 fogColor;
    float4 ambientLight;
    float3 lightStrength;
    float fogstart;
    float fogRange;
    int numOfLights;
    int pad1;
};
Texture2D SRVTexture : register(t0);
Texture2D SRVNormalMap : register(t1);
Texture2D SRVshadowMap : register(t2);
TextureCube cubeMap : register(t4);
Texture2D SSAOAmbientMap : register(t5);

//random vector map
SamplerState smpLinearWrap : register(s0);
SamplerState smpAnisotropicWrap : register(s1);
SamplerComparisonState smpShadowMap : register(s2);
//only for normal depth map
SamplerState smpNormalMap : register(s3);
//blur map
SamplerState smpLinearClamp : register(s4);


float4 main(VertexOut pin) : SV_TARGET
{
    // Fetch the material data.
    float4 diffuseAlbedo = mat.diffuseAlbedo;
    float3 fresnelR0 = mat.fresnelR0;
    float roughness = 1.0f - mat.shininess;
	
	// Interpolating normal can unnormalize it, so renormalize it.
    pin.NormalW = normalize(pin.NormalW);
    float4 normalMapSample = SRVNormalMap.Sample(smpAnisotropicWrap, pin.Tex);

    // Vector from point being lit to eye. 
    float3 toEyeW = normalize(camPositon - pin.PosW);

    float3 bumpedNormalW = NormalSampleToWorldSpace(normalMapSample.rgb, pin.NormalW, pin.tangentW);
    
	// Dynamically look up the texture in the array.
    diffuseAlbedo *= SRVTexture.Sample(smpAnisotropicWrap, pin.Tex);
    if (alphaClip)
    {
        clip(diffuseAlbedo.a - 0.1f);
    }
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
        ambientAccess = SSAOAmbientMap.Sample(smpLinearWrap, pin.SSAOPosH.xy, 0.0f).r;
    }
    
    // Light terms.
    float4 ambient = ambientLight * diffuseAlbedo * ambientAccess;

    MaterialEx mat = { diffuseAlbedo, fresnelR0, shininess };
    //float shadowFactor = CalcShadowFactor(smpShadowMap, SRVshadowMap, pin.shadowPosH);
    float shadowFactor = 1.0f;

    
    float attenuation = 0.0f;
    DirectionalLightEx dr;
    float4 litColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    [unroll]
    for (int i = 0; i < numOfLights; i++)
    {
        dr.direction = pin.PosW - lightPos[i].xyz;
        float distance = length(dr.direction);
        dr.strength = lightStrength;
        attenuation = (2.5f / (1.0f + distance * distance)) * randomPart;
        float4 result = float4(shadowFactor * ComputeDirectionalLightEx(dr, mat, bumpedNormalW, toEyeW), 0.0f);
        litColor += (ambient + result) * attenuation;
    }

	// Add in specular reflections.
   
    float3 r = reflect(-toEyeW, bumpedNormalW);
    float4 reflectionColor = cubeMap.Sample(smpAnisotropicWrap, r);
    float3 fresnelFactor = SchlickFresnel(fresnelR0, bumpedNormalW, r);
    
    litColor.rgb += shininess * fresnelFactor * reflectionColor.rgb;
    // Common convention to take alpha from diffuse albedo.
    litColor.a = diffuseAlbedo.a;
    return litColor;
}