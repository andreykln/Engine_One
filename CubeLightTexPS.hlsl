struct DirectionalLight
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float3 direction;
    float pad;
};
Texture2D cubeTexture;
SamplerState cubeSample : register(s0);
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

cbuffer CBPerFrame : register(b1)
{
    DirectionalLight directLight[3];
    float3 eyePosition;
    int padding;
    Material skullMaterial;
};



struct VertexIn
{
    float3 position : Position;
    float3 normal : Normal;
    float2 tex : TexCoordinate;
};

struct VertexOut
{
    float4 PosH : SV_Position;
    float3 PosW : Position;
    float3 NormalW : Normal;
    float2 tex : TexCoordinate;
};

float4 main(VertexOut pin) : SV_TARGET
{
    // Interpolating normal can unnormalize it
    pin.NormalW = normalize(pin.NormalW);
    
    // The toEye vector is used in lighting.
    float3 toEye = eyePosition - pin.PosW;
    
    // Cache the distance to the eye from this surface point.
    float distToEye = length(toEye);
    
    //normalize
    toEye /= distToEye;
    
    ////////////////////////////////////////
    float4 texColor = float4(1, 1, 1, 1);
    texColor = cubeTexture.Sample(cubeSample, pin.tex);
    
    
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    [unroll]
    for (int i = 0; i < padding; ++i)
    {
        float4 A, D, S;
        ComputeDirectionalLight(skullMaterial, directLight[i], pin.NormalW, toEye, A, D, S);
        ambient += A;
        diffuse += D;
        specular += S;
    }
    
    
    float4 litColor = texColor * (ambient + diffuse) + specular;

    // Common to take alpha from diffuse material and texture
    litColor.a = skullMaterial.diffuse.a * texColor.a;
    
    return litColor;
}