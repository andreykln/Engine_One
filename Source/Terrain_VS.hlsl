struct VertexIn
{
    float3 posL : POSITION;
    float2 tex : TEXCOORD0;
    float2 boundsY : TEXCOORD1;
};


struct VertexOut
{
    float3 posW : POSITION;
    float2 tex : TEXCOORD0;
    float2 boundsY : TEXCOORD1;
};

Texture2D heightMap : register(t0);

//random vector map
SamplerState smpLinearWrap : register(s0);
SamplerState smpAnisotropicWrap : register(s1);
SamplerComparisonState smpShadowMap : register(s2);
//only for normal depth map
SamplerState smpNormalMap : register(s3);
//blur map
SamplerState smpLinearClamp : register(s4);

VertexOut main(VertexIn vin)
{
    VertexOut vout;
    
    vout.posW = vin.posL;
    
    //displace the patch corners to world space. This will make
    // the eye to patch distance calculation more precise
    vout.posW.y = heightMap.SampleLevel(smpLinearWrap, vin.tex, 0).r;
    vout.tex = vin.tex;
    vout.boundsY = vin.boundsY;
    return vout;
}