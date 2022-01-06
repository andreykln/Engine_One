
cbuffer world : register(b0)
{
    float4x4 world;
    float4x4 viewProjection;
    float4x4 worldInvTranspose;
    float4x4 texTransform;
    float4x4 shadowTransform;
    float4x4 matTransform;
    float4x4 worldViewProjTex[10];
    float3 cameraPositon;
    int pad0;
    bool enableDisplacementMapping = false;
    int pad1;
    int pad2;
    int pad3;
};

struct VertexIn
{
    float3 pos : Position;
    float3 normal : Normal;
    float2 texC : TexCoordinate;
};

struct VertexOut
{
    float4 posH : SV_Position; 
    float3 posW : POSITION;
    float3 NormalW : Normal;
    float2 Tex : TEXCOORD0;
};
                                     
VertexOut main(VertexIn vin)
{
    VertexOut vout;
    vout.posH = mul(float4(vin.pos, 1.0f), viewProjection);
    vout.posW = mul(float4(vin.pos, 1.0f), world).xyz;
    vout.NormalW = mul(float4(vin.normal, 1.0f), world).xyz;
    vout.Tex = mul(float4(vin.texC, 0.0f, 1.0f), texTransform).xy;
	return vout;
}


