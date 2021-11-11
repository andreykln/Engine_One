

struct DirectionalLight
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float3 direction;
    float pad;
};

struct PointLight
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    
    float3 position;
    float range;
    
    float3 attenuation;
    float pad;
};

struct SpotLight
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    
    float3 position;
    float range;
    
    float3 direction;
    float spot;
    
    float3 attenuation;
    float pad;
};

struct Material
{
    float4 ambient;
    float4 diffuse;
    float4 specular; // w = specularity power
    float4 reflect;
};

// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a directional light.  We need to output the terms separately because
// later we will modify the individual terms.

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



void ComputePointLight (Material mat, PointLight L,
                        float3 position, float3 normal,
                        float3 toEye,
                        out float4 ambient,
                        out float4 diffuse,
                        out float4 specular)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    //The vector from surface point to the light
    float3 lightVector = L.position - position;
    
    //the distance from surface to light
    float d = length(lightVector);
    
    //range test
    if (d > L.range)
    {
        return;
    }
    
    //normalize the light vector
    lightVector /= d;
    
    //ambient term
    ambient = mat.ambient * L.ambient;
    
    //add diffuse and specular term, provided the surface is
    // in the line of sight of the light
    float diffuseFactor = dot(lightVector, normal);
    
    //flatten to avoid dynamic branching
    [flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVector, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);
        
        diffuse = diffuseFactor * mat.diffuse * L.diffuse;
        specular = specFactor * mat.specular * L.specular;
    }
     //attenuate
    float att = 1.0f / dot(L.attenuation, float3(1.0f, d, d * d));
    
    diffuse *= att;
    specular *= att;
}

void ComputeSpotLight (Material mat, SpotLight L,
                       float3 pos, float3 normal, float3 toEye,
                       out float4 ambient,
                       out float4 diffuse,
                       out float4 specular)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    //vector from the surface to the light
    float3 lightVec = L.position - pos;
    
    //distance from the surface of light
    float d = length(lightVec);
    
    //range test
    if (d > L.range)
    {
        return;
    }
    
    //normalize the light vector
    lightVec /= d;
    
    //ambient term
    ambient = mat.ambient * L.ambient;
    
    // Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.
    
    float diffuseFactor = dot(lightVec, normal);
    
    [flatten]
    if( diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);

        diffuse = diffuseFactor * mat.diffuse * L.diffuse;
        specular = specFactor * mat.specular * L.specular;
    }
    
    //scale by spotlight factor and attenuate
    float spot = pow(max(dot(-lightVec, L.direction), 0.0f), L.spot);
    
    //scale by spotlight factpr and attenuate
    float att = spot / dot(L.attenuation, float3(1.0f, d, d * d));
    
    ambient *= spot;
    diffuse *= att;
    specular *= att;

}


float3 NormalSampleToWorldSpace(float3 normalMapSample,
                                float3 unitNormalW,
                                float3 tangentW)
{
    //Uncompress each component from [0,1] tp [-1, 1]
    float3 normalT = 2.0f * normalMapSample - 1.0f;
    
    //build orthonormal basis
    float3 N = unitNormalW;
    float3 T = normalize(tangentW - dot(tangentW, N) * N);
    float3 B = cross(N, T);
    
    float3x3 TBN = float3x3(T, B, N);
    
    //transform from tangent space to world space
    float3 bumpedNormalW = mul(normalT, TBN);
    
    return bumpedNormalW;
}
static const float SMAP_SIZE = 2048.0f;
static const float SMAP_DX = 1.0f / SMAP_SIZE;
float CalcShadowFactor(SamplerComparisonState samShadow, Texture2D shadowMap, float4 shadowPosH)
{
    shadowPosH.xyz /= shadowPosH.w;
    float depth = shadowPosH.z;
    const float dx = SMAP_DX;
    float percentLit = 0.0f;
    const float2 offsets[9] =
    {
        float2(-dx, -dx), float2(0.0f, -dx), float2(dx, -dx),
        float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
        float2(-dx, dx), float2(0.0f, dx), float2(dx, dx)
    };
    
    //3x3 box filter pattern. Each sample does a 4-tap PCF
    [unroll]
    for (int i = 0; i < 9; ++i)
    {
        percentLit += shadowMap.SampleCmpLevelZero(samShadow,
        shadowPosH.xy + offsets[i], depth).r;
    }
    return percentLit /= 9.0f;
}