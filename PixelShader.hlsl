
// try to put PS into VS


/*struct VertexIn
{
    float4 col : Color;
    float4 Position : SV_Position;

};*/

float4 main(float3 col : Color) : SV_TARGET
{
    return float4(col, 1.0f);
}

