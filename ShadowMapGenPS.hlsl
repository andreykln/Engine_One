struct VertexOut
{
    float4 posH : SV_POSITION;
    float2 tex : TEXCOORD;
};

Texture2D diffuseMap : register(t0);
SamplerState samplerLinear : register(s0);


void main(VertexOut pin)
{
    float4 diff = diffuseMap.Sample(samplerLinear, pin.tex);
    //don't write transparent pixels to shadow map
    clip(diff.a - 0.15f);
}