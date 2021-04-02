struct DirectionalLight
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float3 direction;
    float pad;
};


struct Material
{
    float4 ambient;
    float4 diffuse;
    float4 specular; // w = specularity power
    float4 reflect;
};

void ComputeDirectionalLight(Material mat, DirectionalLight L,
                               float3 normal, float3 toEye,
                                out float4 ambient,
                                out float4 diffuse,
                                out float4 specular)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    //The light vector aims opposite the directoion the light rays travel
    float3 lightVector = -L.direction;
    
    ambient = mat.ambient * L.ambient;
    
    // Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.
    float diffuseFactor = dot(lightVector, normal);
    
    // Flatten to avoid dynamic branching.
	[flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVector, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);
					
        diffuse = diffuseFactor * mat.diffuse * L.diffuse;
        specular = specFactor * mat.specular * L.specular;
    }
}

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
    unsigned int textureArrPos;
}

Texture2DArray SRVTexture : register(t0);
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
    float3 toEye = camPositon - pin.PosW;
    
    // Cache the distance to the eye from this surface point.
    float distToEye = length(toEye);
    
    //normalize
    toEye /= distToEye;

    float4 texColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    texColor = SRVTexture.Sample(tex0Sample, float3(pin.Tex, textureArrPos));
    
    clip(texColor.r - 0.001f); //clip pure black pixels for lightning.
    clip(texColor.g - 0.001f);
    clip(texColor.b - 0.001f);

    float4 litColor = texColor;

    if (numberOfLights > 0)
    {
    
        float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
        [unroll]
        for (uint i = 0; i < numberOfLights; ++i)
        {
            float4 A, D, S;
            ComputeDirectionalLight(objectMaterial, directLight[i], pin.NormalW, toEye, A, D, S);
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
    litColor.a = objectMaterial.diffuse.a * texColor.a;

    return litColor;
}