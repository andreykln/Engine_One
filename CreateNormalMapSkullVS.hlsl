
struct VertexIn
{
    float3 position : Position;
    float3 normal : Normal;
};


struct VertexOut
{
    float4 posH : SV_POSITION;
    float3 posV : POSITION;
    float3 normalV : NORMAL;
};

cbuffer cbDefaultVS : register(b0)
{
    float4x4 worldView;
    float4x4 worldInvTransposeView;
    float4x4 worldViewProjection;
};

VertexOut main(VertexIn vin)
{
   
    VertexOut vout;
	
	// Transform to view space.
    vout.posV = mul(float4(vin.position, 1.0f), worldView).xyz;
    vout.normalV = mul(vin.normal, (float3x3) worldInvTransposeView);
		
	// Transform to homogeneous clip space.
    vout.posH = mul(float4(vin.position, 1.0f), worldViewProjection);
	

 
    return vout;
}