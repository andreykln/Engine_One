cbuffer cbPerFrame
{
    float4x4 lightWVP;
    float4x4 texTransform;
};

struct VertexIn
{
    float3 posL : POSITION;
    float2 tex : TEXCOORD;
};

struct VertexOut
{
    float4 posH : SV_POSITION;
    float3 normalL : NORMAL;
    float2 tex : TEXCOORD;
};


VertexOut main( VertexIn vin )
{
    VertexOut vout;
    vout.posH = mul(float4(vin.posL, 1.0f), lightWVP);
    vout.tex = mul(float4(vin.tex, 0.0f,1.0f), texTransform);
	return vout;
}