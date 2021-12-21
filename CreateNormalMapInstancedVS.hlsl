cbuffer cbDefaultVS : register(b0)
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
};

/*cbuffer SSAOWorlds : register(b1)
{
    float4x4 SSAOworld[10];
}*/

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
    float4 posH : SV_Position;
    float3 normalW : NORMAL;
};



VertexOut main(VertexIn vin)
{
    VertexOut vout;
    
    //float4x4 WIT = mul(SSAOworld[vin.InstanceId], worldInverseTranspose);
    float4x4 WIT = mul(vin.world, worldInverseTranspose);
    
    vout.normalW = mul(vin.normal, (float3x3) WIT);
    
    float4x4 worldView = mul(vin.world, viewProjection);
    
    vout.posH = mul(float4(vin.position, 1.0f), worldView);
    
    return vout;
}