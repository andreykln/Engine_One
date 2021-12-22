struct VertexIn
{
    float3 posL : Position;
    float3 normalL : Normal;
    float2 texC : TexCoordinate;
    float3 tangentL : Tangent;
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

VertexOut main (VertexIn vin)
{
    VertexOut vout;
    vout.normalW = mul(vin.normalL, (float3x3) worldInverseTranspose);
    
    float4x4 worldView = mul(world, viewProjection);
    
    vout.posH = mul(float4(vin.posL, 1.0f), worldView);
    
    return vout;
}