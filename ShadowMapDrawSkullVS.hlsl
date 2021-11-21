cbuffer CBPerObject : register(b0)
{
    float4x4 world;
    float4x4 worldViewProjection;
    float4x4 worldInverseTranspose;
    float4x4 texTransform;
    float4x4 shadowTransform;
};

struct VertexIn
{
    float3 position : Position;
    float3 normal : Normal;
};

struct VSout
{
    float4 PosH : SV_Position;
    float3 PosW : Position;
    float3 NormalW : Normal;
    float4 shadowPosH : TEXCOORD1;
};


VSout main(VertexIn vin)
{
    VSout vout;
    // Transform to world space.
    vout.PosH = mul(float4(vin.position, 1.0f), worldViewProjection);
    vout.PosW = mul(float4(vin.position, 1.0f), world).xyz;
    vout.NormalW = mul(float4(vin.normal, 1.0f), worldInverseTranspose).xyz;
    
    vout.shadowPosH = mul(float4(vin.position, 1.0f), shadowTransform);
    return vout;
}