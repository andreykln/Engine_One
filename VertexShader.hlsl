

struct VertexIn
{
    float4 Position : SV_Position;
    float4 Color : COLOR;
};

struct VertexOut
{
    float4 Position : SV_Position;
    float4 Color : COLOR;
};

VertexOut main(VertexIn vin)
{
    VertexOut vout;
    vout.Position = vin.Position;
    vout.Color = vin.Color;
	return vout;
}