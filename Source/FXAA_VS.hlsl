struct VertexIn
{
    float3 pos : Position;
};
struct VertexOut
{
    float4 pos : SV_Position;
    float2 texC : TEXCOORD;
};

cbuffer cbDefaultMatricesVS : register(b0)
{
    float4x4 world;
    float4x4 viewProjection;
    float4x4 worldInvTranspose;
    float4x4 texTransform;
    float4x4 shadowTransform;
    float4x4 matTransform;
    float4x4 worldViewProjTex;
    float3 cameraPosition;
    int pad0;
    bool enableDisplacementMapping;
}

VertexOut main(VertexIn vin)
{
    VertexOut vout;
    vout.pos = float4(vin.pos.xy, 0.0f, 1.0f);
    
    float2 xy = 0.5f * float2(vin.pos.xy.x, -vin.pos.xy.y);
   
    vout.texC = xy + float2(0.5f, 0.5f);
    return vout;
}