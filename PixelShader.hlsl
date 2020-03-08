
// try to put PS into VS


struct VertexIn
{
    float4 Position : SV_Position;
    float4 Color : COLOR;
};

float4 main(VertexIn vin) : SV_Target
{
	return vin.Color;
}