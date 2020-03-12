
struct VertexOut
{
    float4 Position : SV_Position; //TODO this order matters

    float3 col : Color; 
};
                                     
VertexOut main(float2 pos : Position, float3 col : Color)
{
    VertexOut vout;
    vout.Position = float4(pos.x, pos.y, 0.0f, 1.0f);
    vout.col = col; 
	return vout;
}


