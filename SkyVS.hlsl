
cbuffer WVP : register(b0)
{
    float4x4 worldViewProjection;
}

struct VertexIn
{
    float3 pos : Position;
};

struct VertexOut
{
    float4 PosH : SV_Position;
    float3 PosL : Position;
};

VertexOut main(VertexIn vin)
{
    VertexOut vout;
    vout.PosH = mul(float4(vin.pos, 1.0f), worldViewProjection).xyww;
    vout.PosL = vin.pos;
    return vout;
}