
struct VertexOut
{
    float4 PosH : SV_Position;
    float3 PosL : Position;
};


TextureCube cubeMap : register(t3);

SamplerState linearSampler : register(s0);


float4 main(VertexOut vin) : SV_TARGET
{
    float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    color = cubeMap.Sample(linearSampler, vin.PosL);
    
    return color;
}