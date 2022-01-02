struct VertexIn
{
    float3 pos : Position;
    float3 normal : Normal;
    float2 texC : TexCoordinate;
};

struct VertexOut
{
    float4 posH : SV_Position;
    float2 texC : TEXCOORD;
};

VertexOut main(VertexIn vin)
{
    VertexOut vout;
    
    //already in NDC space
    vout.posH = float4(vin.pos, 1.0f);
    vout.texC = vin.texC;
    
	return vout;
}