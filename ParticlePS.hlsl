struct GSOutput
{
    float4 posH : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
};

Texture2D SRVTexture : register(t0);
SamplerState tex0Sample : register(s0);

float4 main(GSOutput pin) : SV_TARGET
{
   // float4 color = SRVTexture.Sample(tex0Sample, float3(pin.tex, 0.0f)) * pin.color;
    float4 color = SRVTexture.Sample(tex0Sample, pin.tex) * pin.color;
    return color;
}