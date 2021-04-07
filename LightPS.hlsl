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
    Material mat;
    float4 fogColor;
    float fogStart;
    float fogRange;
    float2 padding;
};

cbuffer PS_Per_Frame : register(b1)
{
    float3 cameraPositon;
    unsigned int numberOfLights;
}
struct VertexIn
{
    float3 position : Position;
    float3 normal : Normal;
};

struct VertexOut
{
    float4 PosH : SV_Position;
    float3 PosW : Position;
    float3 NormalW : Normal;
};

float4 main(VertexOut pin) : SV_TARGET
{
    // Interpolating normal can unnormalize it
    pin.NormalW = normalize(pin.NormalW);
    
    // The toEye vector is used in lighting.
    float3 toEye = cameraPositon - pin.PosW;
    
    // Cache the distance to the eye from this surface point.
    float distToEye = length(toEye);
    
    //normalize
    toEye /= distToEye;
    
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
    float4 litColor = ambient + diffuse + specular;
    
    litColor.a = mat.diffuse.a;
    
    return litColor;
}