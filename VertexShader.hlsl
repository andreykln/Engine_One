cbuffer cbPerOdject
{
    float4x4 WorldViewProj;
};

struct VertexIn
{
    float3 Position : SV_Position;
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
    vout.Position = mul(float4(vin.Position, 1.0f), WorldViewProj);
    vout.Color = vin.Color;
	return vout;
}