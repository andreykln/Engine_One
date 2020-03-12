
// try to put PS into VS



float4 main(float3 col : Color) : SV_TARGET
{
    return float4(col, 1.0f);
}

