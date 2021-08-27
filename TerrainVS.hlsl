Texture2D heightMap : register(t0);
SamplerState samLinear : register(s0);


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

VertexOut main(VertexIn vin)
{
    VertexOut vout;
    
    vout.posW = vin.posL;
    
    //displace the patch corners to world space. This will make
    // the eye to patch distance calculation more precise
    vout.posW.y = heightMap.SampleLevel(samLinear, vin.tex, 0).r;
    vout.tex = vin.tex;
    vout.boundsY = vin.boundsY;
	return vout;
}