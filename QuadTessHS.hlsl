cbuffer Matrices
{
    float4x4 World;
    float3 cameraPosition;
};


struct VertexOut
{
    float3 posLocal : Position;
};



struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[4]			: SV_TessFactor;
	float InsideTessFactor[2]		: SV_InsideTessFactor;
};


// Patch Constant Function
HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<VertexOut, 4> patch,
	uint patchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT Output;
	
	//find center of the patch in world space
    float3 centerL = 0.25f * (patch[0].posLocal + patch[1].posLocal + patch[2].posLocal + patch[3].posLocal);
    float3 centerW = mul(float4(centerL, 1.0f), World).xyz;
    float d = distance(centerW, cameraPosition);
	
	//tesselate the patch based on the distance
    const float d0 = 20.0f;
    const float d1 = 100.0f;
	
    float tess = 64.0f * saturate((d1 - d) / (d1 - d0));
	
    Output.EdgeTessFactor[0] = tess;
    Output.EdgeTessFactor[1] = tess;
    Output.EdgeTessFactor[2] = tess;
    Output.EdgeTessFactor[3] = tess;
	
    Output.InsideTessFactor[0] = tess;
    Output.InsideTessFactor[1] = tess;
	
	return Output;
}


// Output control point
struct HS_CONTROL_POINT_OUTPUT
{
    float3 vPosition : Position;
};

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("CalcHSPatchConstants")]
[maxtessfactor(64.0f)]
HS_CONTROL_POINT_OUTPUT main( 
	InputPatch<VertexOut, 4> ip,
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
	HS_CONTROL_POINT_OUTPUT Output;

	Output.vPosition = ip[i].posLocal;

	return Output;
}
