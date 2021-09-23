
struct Particle
{
    float3 initialPosW : Position;
    float3 initialVelocityW : Velocity;
    float2 size : Size;
    float age : Age;
    int type : Type;
};

Particle main(Particle vin)
{  
    return vin;
}