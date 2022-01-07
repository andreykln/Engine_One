struct VertexIn
{
    float3 posL : Position;
    float3 normalL : Normal;
    float2 texC : TexCoord;
    float3 tanL : Tangent;
};

struct VertexOut
{
    float4 posH : SV_POSITION;
    float2 tex : TEXCOORD;
};


cbuffer cbPerFrame : register(b0)
{
    float4x4 lightWVP;
    float4x4 texTransform;
};

VertexOut main(VertexIn vin)
{
    VertexOut vout;
    vout.posH = mul(float4(vin.posL, 1.0f), lightWVP);
    vout.tex = mul(float4(vin.texC, 0.0f, 1.0f), texTransform);
    return vout;
}