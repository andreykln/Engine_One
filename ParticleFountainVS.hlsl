static const float3 accelW = { -1.0f, 9.8f, 0.0f };

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
    uint type : TYPE;
};

VertexOut main(Particle vin)
{
    VertexOut vout;
    
    float t = vin.age;
    
    //constant acceleration equation
    vout.posW = 0.5f * t * t * accelW + t * vin.initialVelocityW + vin.initialPosW;
    
    vout.type = vin.type;
    
    return vout;
    
}