struct VertexIn
{
    float3 posL : Position;
    float3 normalL : Normal;
    float2 texC : TexCoord;
    float3 tanL : Tangent;
    float3 boneW : BoneWeights;
    uint4 boneIndices : BoneIndices;
};

struct VertexOut
{
    float4 PosH : SV_Position;
    float3 PosW : Position;
    float3 NormalW : Normal;
    float3 tangentW : TANGENT;
    float2 Tex : TEXCOORD0;
    float4 shadowPosH : TEXCOORD1;
    float4 SSAOPosH : TEXCOORD2;
};

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


cbuffer cbSkinned : register(b1)
{
    float4x4 boneTransforms[96];
};

VertexOut main(VertexIn vin)
{
    VertexOut vout;
    
    float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    weights[0] = vin.boneW.x;
    weights[1] = vin.boneW.y;
    weights[2] = vin.boneW.z;
    weights[3] = 1.0f - weights[0] - weights[1] - weights[2];

    float3 posL = float3(0.0f, 0.0f, 0.0f);
    float3 normalL = float3(0.0f, 0.0f, 0.0f);
    float3 tangentL = float3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < 4; ++i)
    {
        // Assume no nonuniform scaling when transforming normals, so 
        // that we do not have to use the inverse-transpose.

        posL += weights[i] * mul(float4(vin.posL, 1.0f), boneTransforms[vin.boneIndices[i]]).xyz;
        normalL += weights[i] * mul(vin.normalL, (float3x3) boneTransforms[vin.boneIndices[i]]);
        tangentL += weights[i] * mul(vin.tanL.xyz, (float3x3) boneTransforms[vin.boneIndices[i]]);
    }
    
    
        // Transform to world space.
    float4 posW = mul(float4(posL, 1.0f), world);
    vout.PosW = posW.xyz;

    // Assumes nonuniform scaling; otherwise, need to use inverse-transpose of world matrix.
    vout.NormalW = mul(normalL, (float3x3) world);
	
    vout.tangentW = mul(tangentL, (float3x3) world);

    // Transform to homogeneous clip space.
    vout.PosH = mul(posW, viewProjection);
	
	// Output vertex attributes for interpolation across triangle.
    float4 texC = mul(float4(vin.texC, 0.0f, 1.0f), texTransform);
    vout.Tex = mul(texC, matTransform).xy;

    // Generate projective tex-coords to project shadow map onto scene.
    vout.shadowPosH = mul(posW, shadowTransform);
    vout.SSAOPosH = mul(float4(posL, 1.0f), worldViewProjTex);
    
    return vout;
}