
struct VertexIn
{
    float3 position : Position;
    float3 normal : Normal;
};


struct VertexOut
{
    float4 posH : SV_POSITION;
    float3 posV : POSITION;
    float3 normalV : NORMAL;
};

cbuffer cbDefaultVS : register(b0)
{
    float4x4 worldView;
    float4x4 worldInvTransposeView;
    float4x4 worldViewProjection;
};

VertexOut main(VertexIn vin)
{
    VertexOut vout;
    vout.posV = mul(float4(vin.position, 1.0f), worldView);
    vout.normalV = mul(vin.normal, (float3x3) worldInvTransposeView);
    vout.posH = mul(float4(vin.position, 1.0f), worldViewProjection);
    return vout;
}