
struct VertexOut
{
    float4 PosH : SV_Position;
    float3 PosL : Position;
};


TextureCube cubeMap;

SamplerState samplerState;


float4 main(VertexOut vin) : SV_TARGET
{
    float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    color = cubeMap.Sample(samplerState, vin.PosL);
    
    return color;
}