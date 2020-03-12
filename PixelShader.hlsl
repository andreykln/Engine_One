
// try to put PS into VS



float4 main(float4 pos : Position, float3 col : Color) : SV_TARGET
{
    return float4(col, 1.0f);
}

