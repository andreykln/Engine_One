struct VertexIn
{
    float3 posL : Position;
    float3 normalL : Normal;
    float2 texC : TexCoordinate;
};

static float4x4 W = { 
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f};


struct VertexOut
{
    float4 posH : SV_POSITION;
    float2 tex : TEXCOORD;
};


VertexOut main(VertexIn vin)
{
    VertexOut vout;
    vout.posH = mul(float4(vin.posL, 1.0f), W);
    vout.tex = vin.texC;
	return vout;
}