struct VertexIn
{
    float3 posL : Position;
    float3 normalL : Normal;
    float2 texC : TexCoord;
    float3 tanL : Tangent;
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
    float3 lightPos[2] : TEXCOORD3;
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

cbuffer cbMultiplePointLight : register(b1)
{
    float4 lightPos[2];
    float4 fogColor;
    float4 ambientLight;
    float fogstart;
    float fogRange;
    int numOfLights;
    int pad1;
};

VertexOut main(VertexIn vin)
{
    VertexOut vout;
        // Transform to world space.
    float4 posW = mul(float4(vin.posL, 1.0f), world);
    vout.PosW = posW.xyz;

    // Assumes nonuniform scaling; otherwise, need to use inverse-transpose of world matrix.
    vout.NormalW = mul(vin.normalL, (float3x3) world);
	
    vout.tangentW = mul(vin.tanL, (float3x3) world);

    // Transform to homogeneous clip space.
    vout.PosH = mul(posW, viewProjection);
	
	// Output vertex attributes for interpolation across triangle.
    float4 texC = mul(float4(vin.texC, 0.0f, 1.0f), texTransform);
    vout.Tex = mul(texC, matTransform).xy;

    // Generate projective tex-coords to project shadow map onto scene.
    vout.shadowPosH = mul(posW, shadowTransform);
    float3 adjustedShadowPos = vin.posL + vin.normalL * 0.2f;
    vout.SSAOPosH = mul(float4(adjustedShadowPos, 1.0f), worldViewProjTex);
    [unroll]
    for (int i = 0; i < numOfLights; i++)
    {
        vout.lightPos[i] = mul(float4(lightPos[i].xyz, 1.0f), world).xyz;
    }
        
    return vout;
}