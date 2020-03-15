
cbuffer world
{
    float4x4 transform;
};

struct VertexOut
{
    float4 col : Color;
    float4 Position : SV_Position; //TODO this order matters

    
};
                                     
VertexOut main(float3 pos : Position, float4 col : Color)
{
    VertexOut vout;
    vout.Position = mul(float4(pos.x, pos.y, pos.z, 1.0f), transform);
    //vout.Position = float4(pos.x, pos.y, pos.z, 1.0f);
    
    vout.col = col; 
	return vout;
}


