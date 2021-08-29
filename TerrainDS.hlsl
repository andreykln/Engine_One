cbuffer perFrame
{
    float4x4 viewProjection;
};

struct DS_OUTPUT
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float2 tex : TEXCOORD0;
    float2 tiledTex : TEXCOORD1;
};

struct HS_CONTROL_POINT_OUTPUT
{
    float3 pos : WORLDPOS;
    float2 tex : TEXCOORD0;
};

// Output patch constant data.
struct HS_CONSTANT_DATA_OUTPUT
{
    float EdgeTessFactor[4] : SV_TessFactor; // e.g. would be [4] for a quad domain
    float InsideTessFactor[2] : SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
};

Texture2D heightMap : register(t0);
SamplerState samLinear : register(s0);

#define NUM_CONTROL_POINTS 4

[domain("quad")]
DS_OUTPUT main(
	HS_CONSTANT_DATA_OUTPUT input,
	float2 uv : SV_DomainLocation,
	const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
{
    const float2 texScale = 50.0f;
	DS_OUTPUT Output;

	//bilinear interpolation
    Output.posW = lerp(
    (patch[0].pos, patch[1].pos),
    (patch[2].pos, patch[3].pos),
    uv.y);
    Output.tex = lerp(
    (patch[0].tex, patch[1].tex),
    (patch[2].tex, patch[3].tex),
    uv.y);
    
    //Tile layer textures over terrain
    Output.tiledTex = Output.tex * texScale;
    
    //displacement mapping
    Output.posH.y = heightMap.SampleLevel(samLinear, Output.tex, 0);
    
    Output.posH = mul(float4(Output.posW, 1.0f), viewProjection);
    
	return Output;
}