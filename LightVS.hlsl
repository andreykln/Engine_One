

cbuffer CBPerObject : register(b0)
{
    float4x4 world;
    float4x4 worldViewProjection;
    float4x4 worldInverseTranspose;
    float4x4 texTransform;
};

struct VertexIn
{
    float3 position : Position;
    float3 normal : Normal;
};

struct VertexOut
{
    float4 PosH : SV_Position;
    float3 PosW : Position;
    float3 NormalW : Normal;
};

VertexOut main(VertexIn vin)
{
    VertexOut vout;
    	// Transform to world space space.
    vout.PosW = mul(float4(vin.position, 1.0f), world).xyz;
    vout.NormalW = mul(vin.normal, (float3x3) worldInverseTranspose);
		
	// Transform to homogeneous clip space.
    vout.PosH = mul(float4(vin.position, 1.0f), worldViewProjection);
    return vout;
}