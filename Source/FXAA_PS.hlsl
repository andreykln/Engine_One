struct VertexOut
{
    float4 pos : SV_Position;
    float2 texC : TEXCOORD;
};


float4 main(VertexOut pin) : SV_TARGET
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}