cbuffer Matrices
{
    float4x4 worldViewProjection;
};

struct DomainOut
{
    float4 vPosition : SV_Position;
};

// Output control point
struct HullOut
{
    float3 vPosition : Position;
};

struct PatchTess
{
    float EdgeTessFactor[4] : SV_TessFactor;
    float InsideTessFactor[2] : SV_InsideTessFactor;
};


[domain("quad")]
DomainOut main(
	PatchTess input,
	float2 uv : SV_DomainLocation,
	const OutputPatch<HullOut, 4> quad)
{
    DomainOut Output;

	//bilinear interpolation
    float3 v1 = lerp(quad[0].vPosition, quad[1].vPosition, 1 - uv.y);
    float3 v2 = lerp(quad[3].vPosition, quad[2].vPosition, 1 - uv.y);
    float3 p = lerp(v1, v2, uv.x);
	//displacement mappinng
    p.y = 0.3f * (p.z * sin(p.x) + p.x * cos(p.z));
	
    Output.vPosition = mul(float4(p, 1.0f), worldViewProjection);
	
	return Output;
}
