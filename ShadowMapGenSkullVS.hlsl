cbuffer cbPerFrame
{
    float4x4 lightWVP;
    float4x4 texTransform;
};

struct VertexIn
{
    float3 position : Position;
    float3 normal : Normal;
};

struct VertexOut
{
    float4 posH : SV_POSITION;
};

static float4x4 fMatrix =
{
    0.2f, 0.0, 0.0,  0.0, // row 1
    0.0f, 0.2f, 0.0, 0.0, // row 2
    0.0f, 0.0f, 0.2, 0.0,
    0.0f, 0.0f, 0.0, 1.0,
};
VertexOut main(VertexIn vin)
{
    VertexOut vout;
    vout.posH = mul(float4(vin.position, 1.0f), lightWVP);

    return vout;
}