struct VertexIn
{
    float3 posL : Position;
    float3 normalL : Normal;
    float2 texC : TexCoord;
    float3 tanL : Tangent;
};

struct VertexOut
{
    float4 posH : SV_POSITION;
    float3 posV : POSITION;
    float3 normalV : NORMAL;
};

cbuffer cbCreateNormalMap : register(b0)
{
    float4x4 worldView;
    float4x4 worldInvTransposeView;
    float4x4 worldViewProjection;
};


VertexOut main(VertexIn vin)
{
    VertexOut vout;
    vout.posV = mul(float4(vin.posL, 1.0f), worldView).xyz;
    vout.normalV = mul(vin.normalL, (float3x3) worldInvTransposeView);
		
	// Transform to homogeneous clip space.
    vout.posH = mul(float4(vin.posL, 1.0f), worldViewProjection);
    
    return vout;
}