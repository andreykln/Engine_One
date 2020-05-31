#include "LightHelper.hlsli"


VertexOut main(VertexIn vin)
{
    VertexOut vout;
    vout.PosW = mul(float4(vin.pos, 1.0f), gWorld).xyz;
    vout.NormalW = mul(vin.normal, (float3x3) gWorldInvTranspose);
		
	// Transform to homogeneous clip space.
    vout.PosH = mul(float4(vin.pos, 1.0f), gWorldViewProj);
	return vout;
}