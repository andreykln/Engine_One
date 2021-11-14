struct VertexOut
{
    float4 posH : SV_POSITION;
    float4 color : COLOR;
};




void main(VertexOut pin)
{
    //don't write transparent pixels to shadow map
    clip(pin.color.a - 0.15f);
}