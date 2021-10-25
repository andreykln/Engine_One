#define PT_EMITTER 0
#define PT_FLARE 1
static const float3 accelW = { 0.02f, 0.22f, 0.0f };
cbuffer cbPerFrame
{
    float4x4 viewProjection;
    float3 cameraPosition;
};

struct VertexOut
{
    float3 posW : POSITION;
    uint type : TYPE;
};
struct GSOutput
{
    float4 posH : SV_POSITION;
    float2 tex : TEXCOORD;
};

//this GS expands points into camera facing quads
[maxvertexcount(2)]
void main(
	point VertexOut gin[1],
	inout LineStream<GSOutput> lineStream)
{
   	// do not draw emitter particles.
    if (gin[0].type != PT_EMITTER)
    {
		// Slant line in acceleration direction.
        float3 p0 = gin[0].posW;
        float3 p1 = gin[0].posW + 0.02f;
		
        GSOutput v0;
        v0.posH = mul(float4(p0, 1.0f), viewProjection);
        v0.tex = float2(0.0f, 0.0f);
        lineStream.Append(v0);
		
        GSOutput v1;
        v1.posH = mul(float4(p1, 1.0f), viewProjection);
        v1.tex = float2(1.0f, 1.0f);
        lineStream.Append(v1);
    }
    
    

}