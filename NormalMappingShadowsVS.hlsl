cbuffer cbDefaultVS : register(b0)
{
    float4x4 world;
    float4x4 viewProjection;
    float4x4 worldInverseTranspose;
    float4x4 texTransform;
    float4x4 shadowTransform;
    float4x4 matTransform;
    float4x4 worldViewProjTex[10];
};

struct VertexIn
{
    float3 position : Position;
    float3 normal : Normal;
    float2 texCoord : TexCoordinate;
    float3 tangentLocal : Tangent;
};

struct VSout
{
    float4 PosH : SV_Position;
    float3 PosW : Position;
    float3 NormalW : Normal;
    float3 tangentW : TANGENT;
    float2 Tex : TEXCOORD0;
    float4 shadowPosH : TEXCOORD1;
    float4 SSAOPosH : TEXCOORD2;
};


VSout main(VertexIn vin)
{
    VSout vout;
	
    // Transform to world space.
    float4 posW = mul(float4(vin.position, 1.0f), world);
    vout.PosW = posW.xyz;

    // Assumes nonuniform scaling; otherwise, need to use inverse-transpose of world matrix.
    vout.NormalW = mul(vin.normal, (float3x3) world);
	
    vout.tangentW = mul(vin.tangentLocal, (float3x3) world);

    // Transform to homogeneous clip space.
    vout.PosH = mul(posW, viewProjection);
	
	// Output vertex attributes for interpolation across triangle.
    float4 texC = mul(float4(vin.texCoord, 0.0f, 1.0f), texTransform);
    vout.Tex = mul(texC, matTransform).xy;

    // Generate projective tex-coords to project shadow map onto scene.
    vout.shadowPosH = mul(posW, shadowTransform);
    vout.SSAOPosH = mul(float4(vin.position, 1.0f), worldViewProjTex[0]);
    return vout;
}