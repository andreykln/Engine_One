struct VertexIn
{
    float3 pos : Position;
    float2 texC : TexCoord;
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
   // vout.pos = mul(float4(vin.pos, 1.0f), world);
   // vout.pos = mul(vout.pos, viewProjection);
    vout.pos = float4(vin.pos.xy, 0.0f, 1.0f);
    
    
    vout.texC = mul(float4(vin.texC, 0.0f, 0.0f), texTransform).xy;
    return vout;
}