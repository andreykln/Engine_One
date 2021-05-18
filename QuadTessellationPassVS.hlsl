struct VertexIn
{
    float3 pos : Position;
};

struct VertexOut
{
    float3 posLocal : Position;
};


VertexOut main(VertexIn vin)
{
    VertexOut vsout;
    vsout.posLocal = vin.pos;
	return vsout;
}