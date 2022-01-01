cbuffer cbNormalMapInstanced : register(b0)
{
    float4x4 worldView[10];
    float4x4 worldInvTransposeView[10];
    float4x4 worldViewProjection[10];
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
    

    vout.normalV = mul(vin.normal, (float3x3) worldInvTransposeView[vin.InstanceId]);
    vout.posH = mul(float4(vin.position, 1.0f), worldViewProjection[vin.InstanceId]);
    
    vout.posV = mul(float4(vin.position, 1.0f), worldView[vin.InstanceId]);
    
    return vout;
}