struct VertexOut
{
    float4 posH : SV_POSITION;
    float2 tex : TEXCOORD;
};

Texture2D texToDraw : register(t0);
SamplerState sam : register(s0);

float4 main(VertexOut pin) : SV_TARGET
{
    float4 col = texToDraw.Sample(sam, pin.tex);
    return col;
}