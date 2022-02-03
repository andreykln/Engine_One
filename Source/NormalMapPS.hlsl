struct VertexOut
{
    float4 posH : SV_POSITION;
    float3 posV : POSITION;
    float3 normalV : NORMAL;
};


float4 main(VertexOut pin) : SV_TARGET
{
    pin.normalV = normalize(pin.normalV);
    float4 color = float4(pin.normalV, pin.posV.z);
    return color;
}