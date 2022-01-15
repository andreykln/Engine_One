struct VertexOut
{
    float4 posH : SV_POSITION;
    float2 tex : TEXCOORD;
};

Texture2D texToDraw : register(t5);
//random vector map
SamplerState smpLinearWrap : register(s0);
SamplerState smpAnisotropicWrap : register(s1);
SamplerComparisonState smpShadowMap : register(s2);
SamplerState smpNormalMap : register(s3);
//blur map
SamplerState smpLinearClamp : register(s4);

float4 main(VertexOut pin) : SV_TARGET
{
    float4 c = texToDraw.Sample(smpLinearClamp, pin.tex);
    return float4(c.rrr, 1.0f);
}