
cbuffer cbDefaultMatricesVS : register(b0)
{
    float4x4 world;
    float4x4 viewProjection;
    float4x4 worldInvTranspose;
    float4x4 texTransform;
    float4x4 shadowTransform;
    float4x4 matTransform;
    float4x4 worldViewProjTex;
    float3 cameraPositon;
    int pad0;
    bool enableDisplacementMapping;
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
    vout.PosH = mul(float4(vin.pos, 1.0f), viewProjection).xyww;
    vout.PosL = vin.pos;
    return vout;
}