
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

cbuffer CBPerObject : register(b0)
{
    float4x4 world;
    float4x4 worldViewProjection;
    float4x4 worldInverseTranspose;
    float4x4 texTransform;
    float4x4 shadowTransform;
};

VertexOut main(VertexIn vin)
{
    VertexOut vout;
    vout.normalW = mul(vin.normal, (float3x3) worldInverseTranspose);
    
    
    vout.posH = mul(float4(vin.position, 1.0f), worldViewProjection);
    /*vout.normalW = vin.normal;
    
    
    vout.posH = float4(vin.position, 1.0f);*/
    return vout;
}