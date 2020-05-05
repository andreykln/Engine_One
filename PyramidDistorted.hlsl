
cbuffer world
{
    float4x4 transform;
    float time;
};

struct VertexOut
{
    float4 col : Color;
    float4 Position : SV_Position; //TODO this order matters

    
};
                                     
VertexOut main(float3 pos : Position, float4 col : Color)
{
    VertexOut vout;
    pos.xy += 0.5f * sin(pos.x) * sin(3.0f * time);
    pos.z *= 0.6f + 0.4f * sin(2.0f * time);
    vout.Position = mul(float4(pos, 1.0f), transform);
    vout.col = col;
    return vout;
}