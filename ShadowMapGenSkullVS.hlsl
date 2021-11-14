cbuffer cbPerFrame
{
    float4x4 lightWVP;
    float4x4 texTransform;
};

struct VertexIn
{
    float3 position : Position;
    float3 normal : Normal;
    float4 color : Color;
};

struct VertexOut
{
    float4 posH : SV_POSITION;
    float2 tex : TEXCOORD;
};


VertexOut main(VertexIn vin)
{
    VertexOut vout;
    vout.posH = mul(float4(vin.position, 1.0f), lightWVP);
    vout.tex = float4(1.0f, 1.0f, 1.0f, vin.color.a);
    return vout;
}