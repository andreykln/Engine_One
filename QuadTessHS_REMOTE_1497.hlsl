cbuffer Matrices
{
    float4x4 World;
    float3 cameraPosition;
};


struct VertexOut
{
    float3 posLocal : Position;
};



struct PatchTess
{
	float EdgeTessFactor[4]			: SV_TessFactor;
	float InsideTessFactor[2]		: SV_InsideTessFactor;
};


// Patch Constant Function
PatchTess ConstantHS(
	InputPatch<VertexOut, 16> patch,
	uint patchID : SV_PrimitiveID)
{
    PatchTess Output;
	
	/*//find center of the patch in world space
    float3 centerL = 0.25f * (patch[0].posLocal + patch[1].posLocal + patch[2].posLocal + patch[3].posLocal);
    float3 centerW = mul(float4(centerL, 1.0f), World).xyz;
    float d = distance(centerW, cameraPosition);
	
	//tesselate the patch based on the distance
    const float d0 = 20.0f;
    const float d1 = 100.0f;
	
    float tess = 64.0f * saturate((d1 - d) / (d1 - d0));*/
	
    Output.EdgeTessFactor[0] = 25;
    Output.EdgeTessFactor[1] = 25;
    Output.EdgeTessFactor[2] = 25;
    Output.EdgeTessFactor[3] = 25;
	
	
    Output.InsideTessFactor[0] = 25;
    Output.InsideTessFactor[1] = 25;
	
	
	return Output;
}


// Output control point
struct HullOut
{
    float3 vPosition : Position;
};

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(16)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
HullOut main( 
	InputPatch<VertexOut, 16> ip,
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
	HullOut Output;

	Output.vPosition = ip[i].posLocal;

	return Output;
}
