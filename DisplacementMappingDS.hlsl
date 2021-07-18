struct DomainOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float2 Tex : TEXCOORD;
    float3 TangentW : TANGENT;
};
//static float4x4 test = float4(float4(1.0f, 0.0f, 0.0f, 0.0f), float4(0.0f, 1.0f, 0.0f, 4.0f),
//float4(0.0f, 0.0f, 1.0f, 0.0f), float4(0.0f, 0.0f, 0.0f, 1.0f));

static float4x4 test0 =
{
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, -4.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};


//static float4x4 test0 = float4({1,2,3,4}, {1,2,3,4}, {1,2,3,4}, {1,2,3,4} );
struct PatchTess
{
    float EdgeTessFactor[3] : SV_TessFactor; 
    float InsideTessFactor : SV_InsideTessFactor;
};

struct HullOut
{
    float3 PosW : Position;
    float3 NormalW : Normal;
    float2 Tex : TEXCOORD;
    float3 tangentW : TANGENT;
};

cbuffer CamPos : register(b0)
{
    float4x4 ViewProjection;
    float3 cameraPosition;
};

SamplerState samplerLinear;
Texture2D normalMap : register(t0);

static float heightScale = 0.07f;

[domain("tri")]
DomainOut main(
	PatchTess patchTess,
    float3 bary : SV_DomainLocation,
    const OutputPatch<HullOut, 3> tri)
{
    DomainOut dout;
	
	// Interpolate patch attributes to generated vertices.
    dout.PosW = bary.x * tri[0].PosW + bary.y * tri[1].PosW + bary.z * tri[2].PosW;
    dout.NormalW = bary.x * tri[0].NormalW + bary.y * tri[1].NormalW + bary.z * tri[2].NormalW;
    dout.TangentW = bary.x * tri[0].tangentW + bary.y * tri[1].tangentW + bary.z * tri[2].tangentW;
    dout.Tex = bary.x * tri[0].Tex + bary.y * tri[1].Tex + bary.z * tri[2].Tex;
	
	// Interpolating normal can unnormalize it, so normalize it.
    dout.NormalW = normalize(dout.NormalW);
	
	//
	// Displacement mapping.
	//
	
	// Choose the mipmap level based on distance to the eye; specifically, choose
	// the next miplevel every MipInterval units, and clamp the miplevel in [0,6].
    const float MipInterval = 20.0f;
    float mipLevel = clamp((distance(dout.PosW, cameraPosition) - MipInterval) / MipInterval, 0.0f, 6.0f);
	
	// Sample height map (stored in alpha channel).
    float h = normalMap.SampleLevel(samplerLinear, dout.Tex, mipLevel).a;
	
	// Offset vertex along normal.
    dout.PosW += (heightScale * (h - 1.0)) * dout.NormalW;
	
	// Project to homogeneous clip space.

    dout.PosH = mul(float4(dout.PosW, 1.0f), ViewProjection);


    return dout;
}
