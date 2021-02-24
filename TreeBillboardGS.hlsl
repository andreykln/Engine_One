struct VertexOut
{
    float3 center : Position;
    float2 size : Size;
};

struct GeometryOut
{
    float4 posH : SV_Position;
    float3 posW : Position;
    float3 normalW : Normal;
    float2 tex : TEXCOORD;
    uint PrimID : SV_PrimitiveID;
};

struct GSOutput
{
	float4 pos : SV_POSITION;
};

[maxvertexcount(3)]
void main(
	triangle float4 input[3] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.pos = input[i];
		output.Append(element);
	}
}