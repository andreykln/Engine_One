
cbuffer SSAOConstBuffer : register(b1)
{
    float4 offsetVectors[14];
    float4 frustumFarCorners[4];

    const float gOcclusionRadius;
    const float gOcclusionFadeStart;
    const float gOcclusionFadeEnd;
    const float gSurfaceEpsilon;
};

struct VertexIn
{
    float3 PosL : Position;
    float3 ToFarPlaneIndex : Normal;
    float2 Tex : TexCoordinate;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 ToFarPlane : TEXCOORD0;
    float2 Tex : TEXCOORD1;
};

VertexOut main(VertexIn vin)
{
    VertexOut vout;
    //already in NDC
    vout.PosH = float4(vin.PosL, 1.0f);
    vout.Tex = vin.Tex;
    
   	//store the index to the frustum corner in the normal x-coord slot.
    vout.ToFarPlane = frustumFarCorners[vin.ToFarPlaneIndex.x].xyz;
    
    return vout;
}