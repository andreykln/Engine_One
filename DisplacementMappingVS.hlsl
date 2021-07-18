cbuffer CBPerObject : register(b0)
{
    float4x4 world;
    float4x4 worldViewProjection;
    float4x4 worldInverseTranspose;
    float4x4 texTransform;
    float3 cameraPosition;
};

struct VertexIn
{
    float3 position : Position;
    float3 normal : Normal;
    float2 texCoord : TexCoordinate;
    float3 tangentLocal : TangentL;
};
static float maxTessDistance = 1.0f;
static float minTessDistance = 25.0f;
static float minTessFactor = 1.0f;
static float maxTessFactor = 5.0f;


struct VSout
{
    float3 PosW : Position;
    float3 NormalW : Normal;
    float2 Tex : TEXCOORD;
    float3 tangentW : TANGENT;
    float tessFactor : TESS;
};


VSout main(VertexIn vin)
{
    VSout vout;
    // Transform to world space.
    vout.PosW = mul(float4(vin.position, 1.0f), world).xyz;
    vout.NormalW = mul(vin.normal, (float3x3) worldInverseTranspose);
    vout.tangentW = mul(vin.tangentLocal, (float3x3)world);
    
	// Transform to homogeneous clip space.
    vout.Tex = mul(float4(vin.texCoord, 0.0f, 1.0f), texTransform).xy;
    
    float d = distance(vout.PosW, cameraPosition);
    
    //normalized tesselation factor
    //the tesselation is 
    // 0 if d >= minTessDistance
    // 1 if d <= maxTessDistance
    float tess = saturate((minTessDistance - d) / (minTessDistance - maxTessFactor));
    
    //rescale [0,1] --> [minTessfactor, maxTessFactor]
    vout.tessFactor = minTessFactor + tess * (maxTessFactor - minTessFactor);
    
    
    return vout;
}