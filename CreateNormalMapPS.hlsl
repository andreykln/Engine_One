#include "LightHelper.hlsli"

struct VertexOut
{
    float4 posH : SV_Position;
    float3 normalW : NORMAL;
};


float4 main(VertexOut pin) : SV_TARGET
{

    pin.normalW = normalize(pin.normalW);

    float4 color = float4(pin.normalW, pin.posH.z);
    return color;
}