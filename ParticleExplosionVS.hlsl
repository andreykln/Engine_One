#define PT_EMITTER 0
#define PT_FLARE 1
#define PT_SHELL 2
struct Particle
{
    float3 initialPosW : Position;
    float3 initialVelocityW : Velocity;
    float2 size : Size;
    float age : Age;
    uint type : Type;
};

struct VertexOut
{
    float3 posW : POSITION;
    float2 sizeW : SIZE;
    float4 color : COLOR;
    uint type : TYPE;
};

VertexOut main(Particle vin)
{
    VertexOut vout;
    
    float t = vin.age;
    float accelW = vin.initialVelocityW;
    //constant acceleration equation
    vout.posW = 0.5f * t * t * accelW + t * vin.initialVelocityW + vin.initialPosW;
    vout.sizeW = vin.size;
    vout.type = vin.type;
    //fade color with time
    if(vin.type == PT_FLARE)
    {
        float opacity = 1.0f - smoothstep(0.0f, 1.0f, t / 1.0f);
        vout.color = float4(1.0f, 1.0f, 1.0, opacity);
    }
    else
    {
        vout.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    
    return vout;
    
}