cbuffer CBPerObject : register(b0)
{
    float4x4 world;
    float4x4 worldViewProjection;
    float4x4 worldInverseTranspose;
    float4x4 texTransform;
};

cbuffer CBVSConst : register(b1)
{
    float cbGridSpatialStep;
    float2 cbDisplacementMapTexelSize;
    float padding;
}

Texture2D displacementMap : register(t0);
SamplerState samplerClamp : register(s0);
struct VertexIn
{
    float3 position : Position;
    float3 normal : Normal;
    float2 texCoord : TexCoordinate;

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
	
    float gGridSpatialStep = cbGridSpatialStep;
    float2 displacementMapTexelSize = cbDisplacementMapTexelSize;
	// Sample the displacement map using non-transformed [0,1]^2 tex-coords.
    vin.position.y = displacementMap.SampleLevel(samplerClamp, vin.texCoord, 1.0f).r;
	
	// Estimate normal using finite difference.
	float du = displacementMapTexelSize.x;
	float dv = displacementMapTexelSize.y;
    float l = displacementMap.SampleLevel(samplerClamp, vin.texCoord - float2(du, 0.0f), 0.0f).r;
    float r = displacementMap.SampleLevel(samplerClamp, vin.texCoord + float2(du, 0.0f), 0.0f).r;
    float t = displacementMap.SampleLevel(samplerClamp, vin.texCoord - float2(0.0f, dv), 0.0f).r;
    float b = displacementMap.SampleLevel(samplerClamp, vin.texCoord + float2(0.0f, dv), 0.0f).r;
    vin.normal = normalize(float3(-r + l, 2.0f * gGridSpatialStep, b - t));
	

	
    vout.PosW = mul(float4(vin.position, 1.0f), world).xyz;
    vout.NormalW = mul(vin.normal, (float3x3) worldInverseTranspose);
		
	// Transform to homogeneous clip space.
    vout.PosH = mul(float4(vin.position, 1.0f), worldViewProjection);
    vout.Tex = mul(float4(vin.texCoord, 0.0f, 1.0f), texTransform).xy;

    return vout;
}