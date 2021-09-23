cbuffer cbPerFrame
{
    float4x4 viewProjection;
    float3 cameraPosition;
};

struct VertexOut
{
    float3 posW : POSITION;
    float2 sizeW : SIZE;
    float4 color : COLOR;
    uint type : TYPE;
};
struct GSOutput
{
	float4 posH : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
};

//this GS expands points into camera facing quads
[maxvertexcount(4)]
void main(
	point VertexOut gin[1],
	inout TriangleStream< GSOutput > triStream)
{
    float2 quadTexC[4] =
    {
        float2(0.0f, 1.0f),
		float2(1.0f, 1.0f),
		float2(0.0f, 0.0f),
		float2(1.0f, 0.0f)
    };
    float3 look = normalize(cameraPosition - gin[0].posW);
    float3 right = normalize(cross(float3(0, 1, 0), look));
    float3 up = cross(look, right);
    
    //compute triangle strip vertices (quad) in world space
    
    float halfWidth = 0.5f * gin[0].sizeW.x;
    float halfHeight = 0.5f * gin[0].sizeW.y;
    
    float4 v[4];
    v[0] = float4(gin[0].posW + halfWidth * right - halfHeight * up, 1.0f);
    v[1] = float4(gin[0].posW + halfWidth * right + halfHeight * up, 1.0f);
    v[2] = float4(gin[0].posW - halfWidth * right - halfHeight * up, 1.0f);
    v[3] = float4(gin[0].posW - halfWidth * right + halfHeight * up, 1.0f);
    
    GSOutput geoOut;
    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        geoOut.posH = mul(v[i], viewProjection);
        geoOut.color = gin[0].color;
        geoOut.tex = quadTexC[i];
        triStream.Append(geoOut);

    }

}