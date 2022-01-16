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

// Output patch constant data.
struct HS_CONSTANT_DATA_OUTPUT
{
    float EdgeTessFactor[3] : SV_TessFactor; // e.g. would be [4] for a quad domain
    float InsideTessFactor : SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
};

struct DS_OUTPUT
{
    float4 posH : SV_Position;
    float3 posW : Position;
    float3 normalW : Normal;
    float3 tangentW : TANGENT;
    float2 texCoord : TEXCOORD0;
    float2 waveDisplacementTex0 : TEXCOORD1;
    float2 waveDisplacementTex1 : TEXCOORD2;
    float2 waveNormalTex0 : TEXCOORD3;
    float2 waveNormalTex1 : TEXCOORD4;
};
#define NUM_CONTROL_POINTS 3

//random vector map
SamplerState smpLinearWrap : register(s0);
Texture2D heightMap0 : register(t0);
Texture2D heightMap1 : register(t1);

cbuffer cbDefaultMatricesVS : register(b0)
{
    float4x4 world;
    float4x4 viewProjection;
    float4x4 worldInvTranspose;
    float4x4 texTransform;
    float4x4 shadowTransform;
    float4x4 matTransform;
    float4x4 worldViewProjTex;
    float3 cameraPosition;
    int pad0;
    bool enableDisplacementMapping;
}


[domain("tri")]
DS_OUTPUT main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 bary : SV_DomainLocation,
	const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
{
    DS_OUTPUT Output;
  	// Interpolate patch attributes to generated vertices.
    Output.posW = bary.x * patch[0].posW + bary.y * patch[1].posW + bary.z * patch[2].posW;
    Output.normalW = bary.x * patch[0].normalW + bary.y * patch[1].normalW + bary.z * patch[2].normalW;
    Output.tangentW = bary.x * patch[0].tangentW + bary.y * patch[1].tangentW + bary.z * patch[2].tangentW;
    Output.texCoord = bary.x * patch[0].texCoord + bary.y * patch[1].texCoord + bary.z * patch[2].texCoord;
    Output.waveDisplacementTex0 = bary.x * patch[0].waveDisplacementTex0 +
    bary.y * patch[1].waveDisplacementTex0 + bary.z * patch[2].waveDisplacementTex0;
    Output.waveDisplacementTex1 = bary.x * patch[0].waveDisplacementTex1 +
    bary.y * patch[1].waveDisplacementTex1 + bary.z * patch[2].waveDisplacementTex1;
    Output.waveNormalTex0 = bary.x * patch[0].waveNormalTex0 + bary.y * patch[1].waveNormalTex0 + bary.z * patch[2].waveNormalTex0;
    Output.waveNormalTex1 = bary.x * patch[0].waveNormalTex1 + bary.y * patch[1].waveNormalTex1 + bary.z * patch[2].waveNormalTex1;
    
    Output.normalW = normalize(Output.normalW);
    
    // Choose the mipmap level based on distance to the eye; specifically, choose
	// the next miplevel every MipInterval units, and clamp the miplevel in [0,6].
    const float mipInterval = 20.0f;
    float mipLevel = clamp((distance(Output.posW, cameraPosition) - mipInterval) / mipInterval, 0.0f, 6.0f);
    
    float height0 = heightMap0.SampleLevel(smpLinearWrap, Output.waveDisplacementTex0, mipLevel).a;
    float height1 = heightMap1.SampleLevel(smpLinearWrap, Output.waveDisplacementTex1, mipLevel).a;
    
    const float heightScale0 = 0.4f;
    const float heightScale1 = 1.2f;
    
    Output.posW.y += heightScale0 * height0;
    Output.posW.y += heightScale1 * height1;
    
    Output.posH = mul(float4(Output.posW, 1.0f), viewProjection);

    return Output;
}
