
cbuffer world : register(b0)
{
    float4x4 transform;
};

/*cbuffer world1 : register(b1)
{
    float4x4 transform1;
};*/

struct VertexOut
{
    float4 col : Color;
    float4 Position : SV_Position; //this order matters
};
                                     
VertexOut main(float3 pos : Position, float4 col : Color)
{
    VertexOut vout;
    vout.Position = mul(float4(pos, 1.0f), transform);
   // vout.Position = mul(float4(pos, 1.0f), transform1);
    
    vout.col = col; 
	return vout;
}


