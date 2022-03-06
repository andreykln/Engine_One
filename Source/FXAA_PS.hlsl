struct VertexOut
{
    float4 pos : SV_Position;
    float2 texC : TEXCOORD;
};

Texture2D drawingPass : register(t0);

//random vector map
SamplerState smpLinearWrap : register(s0);
SamplerState smpAnisotropicWrap : register(s1);
SamplerComparisonState smpShadowMap : register(s2);
//only for normal depth map
SamplerState smpNormalMap : register(s3);
//blur map
SamplerState smpLinearClamp : register(s4);

float4 main(VertexOut pin) : SV_TARGET
{
    float4 c = drawingPass.Sample(smpAnisotropicWrap, pin.texC);
    
	return c;
}