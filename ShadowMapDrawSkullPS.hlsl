#include "LightHelper.hlsli"

struct VSout
{
    float4 PosH : SV_Position;
    float3 PosW : Position;
    float3 NormalW : Normal;
    float4 shadowPosH : TEXCOORD1;
    float4 color : COLOR;
};

cbuffer CBPSDirectionalLight_Fog : register(b0)
{
    DirectionalLightEx directLightEx;
    float pad0;
    MaterialEx objectMaterial;
    float4 ambientLight;
    float4 fogColor;
    float fogStart;
    float fogRange;
    float pad1;
    float pad2;
    
};

cbuffer PS_Per_FrameShadowMap : register(b1)
{
    float3 lightDirection;
    unsigned int numberOfLights;
    float3 camPositon;

}

Texture2D SRVshadowMap : register(t2);
SamplerComparisonState shadowSampler : register(s1);


float4 main(VSout pin) : SV_TARGET
{
    
        // Interpolating normal can unnormalize it, so renormalize it.
    pin.NormalW = normalize(pin.NormalW);

    // Vector from point being lit to eye. 
    float3 toEyeW = normalize(camPositon - pin.PosW);

	// Indirect lighting.
    float4 ambient = ambientLight * objectMaterial.diffuseAlbedo;

    const float shininess = 1.0f - objectMaterial.roughness;
    MaterialEx mat = { objectMaterial.diffuseAlbedo, objectMaterial.fresnelR0, shininess };
    float3 shadowFactor = 1.0f;
    float4 directLight;
    DirectionalLightEx dr = directLightEx;
    dr.direction = lightDirection;
    directLight = float4(shadowFactor * ComputeDirectionalLightEx(dr, objectMaterial, pin.NormalW, toEyeW),0.0f);

    float4 litColor = ambient + directLight;

    // Common convention to take alpha from diffuse material.
    litColor.a = objectMaterial.diffuseAlbedo.a;

    return litColor;
    
    
   /* //update rotating main light for shadow map
   //Interpolating normal can unnormalize it
    pin.NormalW = normalize(pin.NormalW);
	
    // The toEye vector is used in lighting.
    float3 toEyeW = normalize(camPositon - pin.PosW);
    
    float4 ambient = ambientLight * objectMaterial.diffuseAlbedo;
    
    float shadowFactor = CalcShadowFactor(shadowSampler, SRVshadowMap, pin.shadowPosH);
    
    const float shininess = (1.0f - objectMaterial.roughness);
    MaterialEx mat = { objectMaterial.diffuseAlbedo, objectMaterial.fresnelR0, shininess };
    float4 directLight = float4(shadowFactor * ComputeDirectionalLightEx(directLightEx, objectMaterial, pin.NormalW, toEyeW),1.0f);

    float4 litColor = ambient + directLight;

	// Add in specular reflections.
    float3 r = reflect(-toEyeW, pin.NormalW);
    float3 fresnelFactor = SchlickFresnel(objectMaterial.fresnelR0, pin.NormalW, 0);
    litColor.rgb += shininess * fresnelFactor;
	
    // Common convention to take alpha from diffuse albedo.
    litColor.a = objectMaterial.diffuseAlbedo.a;

    return litColor;*/
    
    
    
    /*// Cache the distance to the eye from this surface point.
    float distToEye = length(toEye);
    
    //normalize
    toEye /= distToEye;*/

    
    
    /*float4 texColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    texColor = pin.color;
  
    float4 litColor = texColor;
    clip(texColor.a - 0.1f);
    
    
    
    float3 shadow = float3(0.0f, 1.0f, 1.0f);
    if (numberOfLights > 0)
    {
    
        float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
        
        shadow[0] = CalcShadowFactor(shadowSampler, SRVshadowMap, pin.shadowPosH);
        
   
        float4 A, D, S;
        ComputeDirectionalLight(objectMaterial, directLight, pin.NormalW, toEye, A, D, S);
        ambient += A;
        diffuse += D * shadow;
        specular += S * shadow;
       
    
    
        litColor = texColor * (ambient + diffuse) + specular;
    }

     //fogging
    float fogLerp = saturate((distToEye - fogStart) / fogRange);
    litColor = lerp(litColor, fogColor, fogLerp);
    // Common to take alpha from diffuse material and texture
    litColor.a = objectMaterial.diffuse.a * texColor.a;
    return litColor;*/
}