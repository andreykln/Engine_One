struct VertexOut
{
    float3 posW : Position;
    float3 normalW : Normal;
    float2 texCoord : TEXCOORD0;
    float2 waveDisplacementTex0 : TEXCOORD1;
    float2 waveDisplacementTex1 : TEXCOORD2;
    float2 waveNormalTex0 : TEXCOORD3;
    float2 waveNormalTex1 : TEXCOORD4;
    float3 tangentW : TANGENT;
    float tessFactor : TESS;
};


// Output patch constant data.
struct HS_CONSTANT_DATA_OUTPUT
{
    float EdgeTessFactor[3] : SV_TessFactor; // e.g. would be [4] for a quad domain
    float InsideTessFactor : SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
};

#define NUM_CONTROL_POINTS 3

// Patch Constant Function
HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<VertexOut, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
    HS_CONSTANT_DATA_OUTPUT Output;

    Output.EdgeTessFactor[0] = 0.5f * (ip[1].tessFactor + ip[2].tessFactor);
    Output.EdgeTessFactor[1] = 0.5f * (ip[2].tessFactor + ip[0].tessFactor);
    Output.EdgeTessFactor[2] = 0.5f * (ip[0].tessFactor + ip[1].tessFactor);
	
    Output.InsideTessFactor = Output.EdgeTessFactor[0];
    return Output;
}

// Output control point
struct HS_CONTROL_POINT_OUTPUT
{
    float3 posW : Position;
    float3 normalW : Normal;
    float2 texCoord : TEXCOORD0;
    float2 waveDisplacementTex0 : TEXCOORD1;
    float2 waveDisplacementTex1 : TEXCOORD2;
    float2 waveNormalTex0 : TEXCOORD3;
    float2 waveNormalTex1 : TEXCOORD4;
    float3 tangentW : TANGENT;
};

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
HS_CONTROL_POINT_OUTPUT main(
	InputPatch<VertexOut, NUM_CONTROL_POINTS> ip,
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID)
{
    HS_CONTROL_POINT_OUTPUT Output;

    Output.posW = ip[i].posW;
    Output.normalW = ip[i].normalW;
    Output.texCoord = ip[i].texCoord;
    Output.tangentW = ip[i].tangentW;
    Output.waveDisplacementTex0 = ip[i].waveDisplacementTex0;
    Output.waveDisplacementTex1 = ip[i].waveDisplacementTex1;
    Output.waveNormalTex0 = ip[i].waveNormalTex0;
    Output.waveNormalTex1 = ip[i].waveNormalTex1;
    return Output;
}
