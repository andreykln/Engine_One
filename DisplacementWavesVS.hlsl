cbuffer perFrame : register(b0)
{
    float4x4 world;
    float4x4 worldViewProjection;
    float4x4 worldInvTranspose;
    float4x4 texTransform;
    float4x4 waveDisplacementTexTransform0;
    float4x4 waveDisplacementTexTransform1;
    float4x4 waveNormalTexTransf0;
    float4x4 waveNormalTexTransf1;
    float3 cameraPosition;
};

static float maxTessDistance = 1.0f;
static float minTessDistance = 25.0f;
static float minTessFactor = 1.0f;
static float maxTessFactor = 5.0f;

struct VertexIn
{
    float3 posL : Position;
    float3 normalL : Normal;
    float2 texCoord : TexCoordinate;
    float3 tangentL : Tangent;
};

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


VertexOut main(VertexIn vin)
{
    VertexOut vout;
    vout.posW = mul(float4(vin.posL, 1.0f), world).xyz;
    vout.normalW = mul(vin.normalL, (float3x3)worldInvTranspose);
    vout.tangentW = mul(vin.tangentL, (float3x3) world);
    vout.waveDisplacementTex0 = mul(float4(vin.texCoord, 0.0f, 1.0f), waveDisplacementTexTransform0).xy;
    vout.waveDisplacementTex1 = mul(float4(vin.texCoord, 0.0f, 1.0f), waveDisplacementTexTransform1).xy;

    vout.waveNormalTex0 = mul(float4(vin.texCoord, 0.0f, 1.0f), waveNormalTexTransf0).xy;
    vout.waveNormalTex1 = mul(float4(vin.texCoord, 0.0f, 1.0f), waveNormalTexTransf1).xy;

    
    // Transform to homogeneous clip space.
    vout.texCoord = mul(float4(vin.texCoord, 0.0f, 1.0f), texTransform).xy;

    float dist = distance(cameraPosition, vout.posW);
    
     //normalized tesselation factor
    //the tesselation is 
    // 0 if d >= minTessDistance
    // 1 if d <= maxTessDistance
    float tess = saturate((minTessDistance - dist) / (minTessDistance - maxTessFactor));
    
    //rescale [0,1] --> [minTessfactor, maxTessFactor]
    vout.tessFactor = minTessFactor + tess * (maxTessFactor - minTessFactor);
    
    return vout;
}