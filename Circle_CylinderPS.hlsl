
struct VertexIn
{
    float4 pos : SV_Position;
    float4 col : color;
};


float4 main(VertexIn vin) : SV_TARGET
{
    float3 psout;
    psout.x = vin.col.x;
    psout.y = vin.col.y;
    psout.z = vin.col.z;
    
    
    
    
    return vin.col;
}