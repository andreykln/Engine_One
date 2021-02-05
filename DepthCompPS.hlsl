
struct VertexIn
{
    float4 pos : SV_Position;
    float4 col : color;
};

float4 main(VertexIn vin) : SV_TARGET
{
    vin.col.x = 1.0f;
    vin.col.y = 1.0f;
    vin.col.z = 1.0f;
    
	return vin.col;
}