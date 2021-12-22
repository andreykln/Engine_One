#include "LightHelper.hlsli"

struct VertexOut
{
    float4 posH : SV_Position;
    //float3 posW : Position;
    float3 normalW : NORMAL;
    //float2 texC : TEXCOORD0;
    //float3 tangentW : TANGENT;
};

/*cbuffer cbDefaultPS : register(b0)
{
    DirectionalLightEx dirLight;
    MaterialEx mat;
    float4 fogColor;
    float4 ambientLight;
    float3 lightDirection;
    float fogstart;
    float3 camPositon;
    float fogRange;
    bool enableNormalMapping;
}
cbuffer cbDefaultVS : register(b1)
{
    float4x4 world;
    float4x4 viewProjection;
    float4x4 worldInverseTranspose;
    float4x4 texTransform;
    float4x4 shadowTransform;
    float4x4 matTransform;
    float3 cameraPosition;
    int pad0;
    bool enableDisplacementMapping;
    int pad1;
    int pad2;
    int pad3;
};*/

float4 main(VertexOut pin) : SV_TARGET
{

    pin.normalW = normalize(pin.normalW);

    float4 color = float4(pin.normalW, pin.posH.z);
    return color;
}