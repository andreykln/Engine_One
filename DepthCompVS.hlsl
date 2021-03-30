
cbuffer CBPerObject : register(b0)
{
    float4x4 world;
    float4x4 worldViewProjection;
    float4x4 worldInverseTranspose;
    float4x4 texTransform;
};


struct VertexIN
{
    float3 pos : Position;
    float4 col : Color;
};

struct VertexOut
{
    float4 pos : SV_Position;
    float4 col : Color;
};

VertexOut main( VertexIN vin )
{
    VertexOut vout;
    vout.pos = mul(float4(vin.pos, 1.0f), worldViewProjection);

    vout.col = vin.col;
    
	return vout;
}