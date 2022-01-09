
struct VertexOut
{
    float4 PosH : SV_Position;
    float3 PosL : Position;
};


TextureCube cubeMap : register(t4);

//random vector map
SamplerState smpLinearWrap : register(s0);

SamplerState smpAnisotropicWrap : register(s1);
SamplerComparisonState smpShadowMap : register(s2);
SamplerState smpNormalMap : register(s3);
//blur map
SamplerState smpLinearClamp : register(s4);


float4 main(VertexOut vin) : SV_TARGET
{
    float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    color = cubeMap.Sample(smpLinearWrap, vin.PosL);
    
    return color;
}