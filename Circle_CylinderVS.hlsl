
cbuffer CBPerObject : register(b0)
{
    float4x4 gWorld;
    float4x4 gWorldInvTranspose;
    float4x4 gWorldViewProj;
};


struct VertexIN
{
    float3 pos : Position;
};

struct VertexOut
{
    float4 pos : SV_Position;
};

VertexOut main(VertexIN vin)
{
    VertexOut vout;
    vout.pos = mul(float4(vin.pos, 1.0f), gWorldViewProj);
   
    return vout;
}
