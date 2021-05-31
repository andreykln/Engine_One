struct VertexIn
{
    float3 posLocal : Position;
    float3 normal : Normal;
    row_major float4x4 world : World;
    float4 color : Color;
};

struct VertexOut
{
    float4 posH : Sv_Position;
    float3 posW : Position;
    float3 normalW : Normal;
    float4 color : Color;
};

cbuffer transform : register(b0)
{
    float4x4 world;
    float4x4 worldViewProjection;
    float4x4 worldInverseTranspose;
    float4x4 texTransform;
};



VertexOut main(VertexIn vin)
{
    VertexOut vout;
    
    // Transform to world space space.
    vout.posW = mul(float4(vin.posLocal, 1.0f), vin.world).xyz;
    vout.normalW = mul(vin.normal, (float3x3) vin.world);
		
	// Transform to homogeneous clip space.
    vout.posH = mul(float4(vout.posW, 1.0f), worldViewProjection);
    
    vout.color = vin.color;
    
    
	return vout;
}