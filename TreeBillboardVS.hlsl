struct VertexIn
{
    float3 pos : Position;
    float2 size : Size;
};

struct VertexOut
{
    float2 size : Size;
    float3 center : Position;
};

VertexOut main(VertexIn vin)
{
    VertexOut vout;
    vout.center = vin.pos;
    vout.size = vin.size;
	return vout;
}