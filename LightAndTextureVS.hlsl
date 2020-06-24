
cbuffer CBPerObject : register(b0)
{
    float4x4 gWorld;
    float4x4 gWorldInvTranspose;
    float4x4 gWorldViewProj;
    float4x4 gTexTransform;
};

struct VertexIn
{
    float3 position : Position;
    float2 texCoord : TexCoordinate;
    float3 normal : Normal;
};

struct PSstruct
{
    float4 PosH : SV_Position;
    float3 PosW : Position;
    float3 NormalW : Normal;
    float2 Tex : TEXCOORD;
};

PSstruct main(VertexIn vin)
{
    PSstruct vout;
    	// Transform to world space space.
    vout.PosW = mul(float4(vin.position, 1.0f), gWorld).xyz;
    vout.NormalW = mul(vin.normal, (float3x3) gWorldInvTranspose);
		
	// Transform to homogeneous clip space.
    vout.PosH = mul(float4(vin.position, 1.0f), gWorldViewProj);
    vout.Tex = mul(float4(vin.texCoord, 0.0f, 1.0f), gTexTransform).xy;
    //vout.Tex = vin.texCoord;
    

    return vout;
}