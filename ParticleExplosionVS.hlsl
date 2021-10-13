
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
    
    return vout;
    
}