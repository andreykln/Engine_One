struct VertexOut
{
    float4 pos : SV_Position;
};

struct GSOutput
{
	float4 pos : SV_POSITION;
};

//extract point to a line
[maxvertexcount(2)]
void main(
	point float4 input[1] : SV_POSITION, 
	inout LineStream< GSOutput > output
)
{
    float4 vertices[2];
	//create a line from a point, by adding the same point but with +1.0f in y
    vertices[0] = float4(input[0].x, 0.0f, input[0].z, 1.0f);
    vertices[1] = float4(input[0].x, 1.0f, input[0].z, 1.0f);
    
    GSOutput element;    
    [unroll]
    for (int i = 0; i < 2; ++i)
    {
        element.pos = vertices[i];
        output.Append(element);
    }
}