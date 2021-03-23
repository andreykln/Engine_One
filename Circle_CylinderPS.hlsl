
struct GSOutput
{
    float4 pos : SV_POSITION;
};

float4 main(GSOutput gin) : SV_TARGET
{
    float4 psout;
    psout.x = 0.5f;
    psout.y = 0.5f;
    psout.z = 0.5f;
    psout.a = 1.0f;
    
    return psout;
}