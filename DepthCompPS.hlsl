cbuffer CBDepthColor : register(b0)
{
    float4 depthColor;
}


struct VertexIn
{
    float4 pos : SV_Position;
    float4 col : color;
};


float4 main(VertexIn vin) : SV_TARGET
{
   vin.col.x = depthColor.x;
   vin.col.y = depthColor.y;
   vin.col.z = depthColor.z;
    
    
    
    return vin.col;
}