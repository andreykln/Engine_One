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
    float3 tangentLocal : Tangent;
};

struct VSout
{
    float4 PosH : SV_Position;
    float3 PosW : Position;
    float3 NormalW : Normal;
    float2 Tex : TEXCOORD;
    float3 tangentW : TANGENT;
};


VSout main(VertexIn vin)
{
    VSout vout;
    // Transform to world space.
    vout.PosH = mul(float4(vin.position, 1.0f), worldViewProjection);
    vout.PosW = mul(float4(vin.position, 1.0f), world).xyz;
    vout.NormalW = mul(float4(vin.normal, 1.0f), worldInverseTranspose).xyz;
    vout.tangentW = mul(vin.tangentLocal, (float3x3) world);
    
	// Transform to homogeneous clip space.
    vout.Tex = mul(float4(vin.texCoord, 0.0f, 1.0f), texTransform).xy;
    

    
    return vout;
}