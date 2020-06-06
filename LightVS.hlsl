

cbuffer CBPerObject : register(b0)
{
    float4x4 gWorld;
    float4x4 gWorldInvTranspose;
    float4x4 gWorldViewProj;
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
    vout.PosW = mul(float4(vin.position, 1.0f), gWorld).xyz;
    vout.NormalW = mul(vin.normal, (float3x3) gWorldInvTranspose);
		
	// Transform to homogeneous clip space.
    vout.PosH = mul(float4(vin.position, 1.0f), gWorldViewProj);
    return vout;
}