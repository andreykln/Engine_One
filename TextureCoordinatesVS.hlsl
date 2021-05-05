cbuffer CBTexCoord : register(b0)
{
    float4x4 texTransform;
};

struct VertexIn
{
    float2 texCoord : TexCoordinate;
};

struct PSstruct
{
    float2 Tex : TEXCOORD;
};
PSstruct main(VertexIn vin)
{
    PSstruct vout;
    vout.Tex = mul(float4(vin.texCoord, 0.0f, 1.0f), texTransform).xy;
    return vout;
}