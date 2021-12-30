cbuffer cbNormalMapInstanced : register(b0)
{
    float4x4 view;
    float4x4 invTransposeView;
    float4x4 viewProjection;
};

struct VertexIn
{
    float3 position : Position;
    float3 normal : Normal;
    float2 texCoord : TexCoordinate;
    float3 tangentLocal : Tangent;
    row_major float4x4 world : WORLD;
    uint InstanceId : SV_InstanceID;
};

struct VertexOut
{
    float4 posH : SV_POSITION;
    float3 posV : POSITION;
    float3 normalV : NORMAL;
};



VertexOut main(VertexIn vin)
{
    VertexOut vout;
    
    float4x4 worldView = mul(vin.world, view);
    float4x4 worldInvTransposeView = mul(vin.world, invTransposeView);
    float4x4 worldViewProjection = mul(vin.world, viewProjection);
    vout.normalV = mul(vin.normal, (float3x3) worldInvTransposeView);
    vout.posH = mul(float4(vin.position, 1.0f), worldViewProjection);
    
    vout.posV = mul(float4(vin.position, 1.0f), worldView);
    
    return vout;
}