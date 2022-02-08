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
    float4 posH : SV_POSITION;
    float2 tex : TEXCOORD;
};


cbuffer cbPerFrame : register(b0)
{
    float4x4 lightWVP;
    float4x4 texTransform;
};

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

    vin.posL = posL;
    vin.normalL = normalL;
    vin.tanL.xyz = tangentL;
    
    
    
    vout.posH = mul(float4(vin.posL, 1.0f), lightWVP);
    vout.tex = mul(float4(vin.texC, 0.0f, 1.0f), texTransform).xy;
    return vout;
}