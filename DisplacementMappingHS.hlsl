struct VSout
{
    float3 PosW : Position;
    float3 NormalW : Normal;
    float2 Tex : TEXCOORD;
    float3 tangentW : TANGENT;
    float tessFactor : TESS;
};



// Output patch constant data.
struct PatchTess
{
	float EdgeTessFactor[3]			: SV_TessFactor; // e.g. would be [4] for a quad domain
	float InsideTessFactor			: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
};


// Patch Constant Function
PatchTess CalcHSPatchConstants(
	InputPatch<VSout, 3> ip,
	uint PatchID : SV_PrimitiveID)
{
    PatchTess Output;


    Output.EdgeTessFactor[0] = 0.5f * (ip[1].tessFactor + ip[2].tessFactor);
    Output.EdgeTessFactor[1] = 0.5f * (ip[2].tessFactor + ip[0].tessFactor);
    Output.EdgeTessFactor[2] = 0.5f * (ip[0].tessFactor + ip[1].tessFactor);
	
    Output.InsideTessFactor = Output.EdgeTessFactor[0];

	return Output;
}

struct HullOut
{
    float3 PosW : Position;
    float3 NormalW : Normal;
    float2 Tex : TEXCOORD;
    float3 tangentW : TANGENT;
};


[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
HullOut main(
	InputPatch<VSout, 3> ip,
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
    HullOut Output;

    Output.PosW = ip[i].PosW;
    Output.NormalW = ip[i].NormalW;
    Output.tangentW = ip[i].tangentW;
    Output.Tex = ip[i].Tex;

	return Output;
}
