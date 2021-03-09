

struct VertexOut
{
    float2 size : Size;
    float3 center : Position;
};

struct GSOutput
{
    float4 posH : SV_Position;
    float3 posW : Position0;
    float3 normalW : Normal;
    float2 tex : TEXCOORD;
    float3 gEyePosition : Position1;
    uint PrimID : SV_PrimitiveID;
};

cbuffer GSBillboard : register(b0)
{
    float4x4 worldProjection;
    float3 eyePosition;
    const int padding;
}

//expand a point to a quad
[maxvertexcount(4)]
void main(point VertexOut gin[1],
              uint primID : SV_PrimitiveID, 
              inout TriangleStream<GSOutput> triStream)
{
    //local coord system of the sprite relative to the world space
    //so the billboard aligned with y and faces the eye
    float3 up = float3(0.0f, 1.0f, 0.0f);
    float3 look = eyePosition - gin[0].center;
    look.y = 0.0f; //y-axis aligned, so project to xz plane
    look = normalize(look);
    float3 right = cross(up, look);
    
    //create a quad from a point
    float halfWidth = 0.5f * gin[0].size.x;
    float halfHeight = 0.5f * gin[0].size.y;
    
    float4 v[4];
    v[0] = float4(gin[0].center + halfWidth * right - halfHeight * up, 1.0f);
    v[1] = float4(gin[0].center + halfWidth * right + halfHeight * up, 1.0f);
    v[2] = float4(gin[0].center - halfWidth * right - halfHeight * up, 1.0f);
    v[3] = float4(gin[0].center - halfWidth * right + halfHeight * up, 1.0f);
    
    float2 gTexC[4] = //it can't be global for some reason, figure out why.
    {
        float2(0.0f, 1.0f),
    float2(0.0f, 0.0f),
    float2(1.0f, 1.0f),
    float2(1.0f, 0.0f)
    };
    //transform quad vertices to world space and output as triangle strip
    GSOutput gout;
    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        gout.posH = mul(v[i], worldProjection);
        gout.posW = v[i].xyz;
        gout.normalW = look;
        gout.tex = gTexC[i];
        gout.PrimID = primID;
        gout.gEyePosition = eyePosition;
        triStream.Append(gout);
    }

}


