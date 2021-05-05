Texture2D SRVTexture : register(t0);
SamplerState Sampler : register(s0);


struct PSstruct
{
    float4 PosH : SV_Position;
    float3 PosW : Position;
    float3 NormalW : Normal;
    float2 Tex : TEXCOORD;
};

float4 main(PSstruct pin) : SV_TARGET
{
    float4 texColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    texColor = SRVTexture.Sample(Sampler, pin.Tex);

    return texColor;
}