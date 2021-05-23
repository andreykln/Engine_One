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
    float EdgeTessFactor[3] : SV_TessFactor;
    float InsideTessFactor[1] : SV_InsideTessFactor;
};


[domain("tri")]
DomainOut main(
	PatchTess input,
	float3 barycentric : SV_DomainLocation,
	const OutputPatch<HullOut, 3> tri)
{
    DomainOut Output;

	//bilinear interpolation
    Output.vPosition.xyz = 
    tri[0].vPosition * barycentric.x + tri[1].vPosition * barycentric.y + tri[2].vPosition * barycentric.z;
    
    
   /* float3 v1 = lerp(quad[0].vPosition, quad[1].vPosition, uvw.x);
    float3 v2 = lerp(quad[1].vPosition, quad[2].vPosition, uvw.x);
    float3 p = lerp(v1, v2, uvw.y);*/
	//displacement mappinng
    //p.y = 0.3f * (p.z * sin(p.x) + p.x * cos(p.z));
	
   // Output.vPosition = mul(float4(p, 1.0f), worldViewProjection);
    Output.vPosition = mul(float4(Output.vPosition.xyz, 1.0f), worldViewProjection);
    
	
	return Output;
}
