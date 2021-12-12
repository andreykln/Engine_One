struct DomainOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float3 TangentW : TANGENT;
    float2 Tex : TEXCOORD;
    float4 shadowPosH : TEXCOORD1;
};


static float4x4 test0 =
{
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, -4.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};


struct PatchTess
{
    float EdgeTessFactor[3] : SV_TessFactor; 
    float InsideTessFactor : SV_InsideTessFactor;
};

struct HullOut
{
    float3 PosW : Position;
    float4 PosH : SV_Position;
    float3 NormalW : Normal;
    float2 Tex : TEXCOORD;
    float3 tangentW : TANGENT;
    float4 shadowPosH : TEXCOORD1;
};

cbuffer CamPos : register(b0)
{
    row_major float4x4 world;
    float4x4 viewProjection;
    float4x4 worldInverseTranspose;
    float4x4 texTransform;
    float4x4 shadowTransform;
    float4x4 matTransform;
    float3 cameraPosition;
    int pad0;
    bool enableDisplacementMapping;
    int pad1;
    int pad2;
    int pad3;
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
    dout.shadowPosH = bary.x * tri[0].shadowPosH + bary.y * tri[1].shadowPosH + bary.z * tri[2].shadowPosH;
    
    
    float4 posW = mul(float4(dout.PosW, 1.0f), world);
 
    
    
    dout.shadowPosH = mul(posW, shadowTransform);
    
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

    dout.PosH = mul(float4(dout.PosW, 1.0f), viewProjection);
    

    return dout;
}
