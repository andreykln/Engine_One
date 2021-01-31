
struct VertexIn
{
    float4 pos : SV_Position;
    float4 col : color;
};

float4 main(VertexIn vin) : SV_TARGET
{
	return vin.col;
}