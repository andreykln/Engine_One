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


cbuffer CBPerFrameMirrorRoom : register(b0)
{
    DirectionalLight directLight[3];
    Material mat;
    float3 cameraPositon;
    uint numberOfLights;
    uint texArrayPos;
    uint padding0;
    uint padding1;
    uint padding2;
};



Texture2D SRVTexture0[3] : register(t0);
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
    float3 toEye = cameraPositon - pin.PosW;
    
    // Cache the distance to the eye from this surface point.
    float distToEye = length(toEye);
    
    //normalize
    toEye /= distToEye;

    float4 texColor = float4(1, 1, 1, 1);
   // texColor = mul(SRVTexture0.Sample(tex0Sample, pin.Tex), SRVTexture1.Sample(tex0Sample, pin.Tex));
    if (texArrayPos == 0)
    {
        texColor = SRVTexture0[0].Sample(tex0Sample, pin.Tex);
    }
    
    if (texArrayPos == 1)
    {
        texColor = SRVTexture0[1].Sample(tex0Sample, pin.Tex);
    }
    if (texArrayPos == 2)
    {
        texColor = SRVTexture0[2].Sample(tex0Sample, pin.Tex);
    }
    
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
            ComputeDirectionalLight(mat, directLight[i], pin.NormalW, toEye, A, D, S);
            ambient += A;
            diffuse += D;
            specular += S;
        }
    
    
        litColor = texColor * (ambient + diffuse) + specular;
    }
    
    // Common to take alpha from diffuse material and texture
    litColor.a = mat.diffuse.a * texColor.a;
    
    return litColor;
}