
struct VertexIn
{
    float3 position : Position;
    float3 normal : Normal;
};


struct VertexOut
{
    float4 posH : SV_Position;
    float3 normalW : NORMAL;
};

cbuffer cbDefaultVS : register(b0)
{
    float4x4 world;
    float4x4 viewProjection;
    float4x4 worldInverseTranspose;
};

VertexOut main(VertexIn vin)
{
    VertexOut vout;
    vout.normalW = mul(vin.normal, (float3x3) worldInverseTranspose);
    vout.posH = mul(float4(vin.position, 1.0f), viewProjection);
    return vout;
}