cbuffer cbPerFrame
{
    float4x4 lightVP;
    float4x4 texTransform;
    
};

struct VertexIn
{
    float3 position : Position;
    float3 normal : Normal;
    float2 texCoord : TexCoordinate;
    float3 tangentLocal : Tangent;
    row_major float4x4 world : WORLD;
};

struct VertexOut
{
    float4 posH : SV_POSITION;
    float2 tex : TEXCOORD;
};


VertexOut main(VertexIn vin)
{
    VertexOut vout;
    float4x4 WVP = mul(vin.world, lightVP);
    vout.posH = mul(float4(vin.position, 1.0f), WVP);
    vout.tex = mul(float4(vin.texCoord, 0.0f, 1.0f), texTransform).xy;
    return vout;
}