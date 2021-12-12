cbuffer cbDefaultVS : register(b0)
{
    float4x4 world;
    float4x4 viewProjection;
    float4x4 worldInverseTranspose;
    float4x4 texTransform;
    float4x4 shadowTransform;
    float4x4 matTransform;
    float3 cameraPosition;
    int pad0;
    bool enableDisplacementMapping;
    int pad1;
    int pad2;
    int pad3;
};

struct VertexIn
{
    float3 position : Position;
    float3 normal : Normal;
    float2 texCoord : TexCoordinate;
    float3 tangentLocal : Tangent;
    row_major float4x4 world : WORLD;
};

struct VSout
{
    float4 PosH : SV_Position;
    float3 PosW : Position;
    float3 NormalW : Normal;
    float3 tangentW : TANGENT;
    float2 Tex : TEXCOORD0;
    float4 shadowPosH : TEXCOORD1;
    float tessFactor : TESS;
};

static float maxTessDistance = 1.0f;
static float minTessDistance = 25.0f;
static float minTessFactor = 1.0f;
static float maxTessFactor = 5.0f;

VSout main(VertexIn vin)
{
    VSout vout;
	
    // Transform to world space.
    float4 posW = mul(float4(vin.position, 1.0f), vin.world);
    vout.PosW = posW.xyz;
    if(enableDisplacementMapping)
    {
        posW = float4(vin.position, 1.0f);
        vout.PosW = posW.xyz;
    }
    

    // Assumes nonuniform scaling; otherwise, need to use inverse-transpose of world matrix.
    vout.NormalW = mul(vin.normal, (float3x3) vin.world);
	
    vout.tangentW = mul(vin.tangentLocal, (float3x3) vin.world);

    // Transform to homogeneous clip space.
    vout.PosH = mul(posW, viewProjection);
	
	// Output vertex attributes for interpolation across triangle.
    float4 texC = mul(float4(vin.texCoord, 0.0f, 1.0f), texTransform);
    vout.Tex = mul(texC, matTransform).xy;

    // Generate projective tex-coords to project shadow map onto scene.
    vout.shadowPosH = mul(posW, shadowTransform);
    
    if(enableDisplacementMapping)
    {
        float d = distance(vout.PosW, cameraPosition);
    
    //normalized tesselation factor
    //the tesselation is 
    // 0 if d >= minTessDistance
    // 1 if d <= maxTessDistance
        float tess = saturate((minTessDistance - d) / (minTessDistance - maxTessFactor));
    //rescale [0,1] --> [minTessfactor, maxTessFactor]
        vout.tessFactor = minTessFactor + tess * (maxTessFactor - minTessFactor);
    }
    else
    {
        vout.tessFactor = 0.0f;
    }
    
    return vout;
}