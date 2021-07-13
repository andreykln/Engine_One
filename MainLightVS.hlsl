cbuffer CBPerObject : register(b0)
{
    float4x4 world;
    float4x4 worldViewProjection;
    float4x4 worldInverseTranspose;
    float4x4 texTransform;
};

struct VertexIn
{
    float3 position : Position;
    float3 normal : Normal;
    float2 texCoord : TexCoordinate;
    float3 tangentLocal : TangentL;
};

struct PSstruct
{
    float4 PosH : SV_Position;
    float3 PosW : Position;
    float3 NormalW : Normal;
    float2 Tex : TEXCOORD;
    float3 tangentW : TANGENT;
};


PSstruct main(VertexIn vin)
{
    PSstruct vout;
    // Transform to world space.
    vout.PosW = mul(float4(vin.position, 1.0f), world).xyz;
    vout.NormalW = mul(vin.normal, (float3x3) worldInverseTranspose);
    vout.tangentW = mul(vin.tangentLocal, (float3x3)world);
    
	// Transform to homogeneous clip space.
    vout.PosH = mul(float4(vin.position, 1.0f), worldViewProjection);
    vout.Tex = mul(float4(vin.texCoord, 0.0f, 1.0f), texTransform).xy;
    
    return vout;
}