/*cbuffer cbPerOdject
{
    float4x4 WorldViewProj;
};*/

/*struct VertexIn
{
   float2 Position : Position;
     float4 col : Color;

};*/

struct VertexOut
{
    float3 col : Color;

    float4 Position : SV_Position;

};

VertexOut main(float2 pos : Position, float3 col : Color)
{
    VertexOut vout;
    vout.Position = float4(pos.x, pos.y, 0.0f, 1.0f);
    
    //vout.Position = float4(vin.Position.x, vin.Position.y, 0.0f, 1.0f);
    //vout.Position = mul(float4(vin.Position, 1.0f), WorldViewProj);
    vout.col = col;
	return vout;
}


