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
