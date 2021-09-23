static const float3 accelW = { 0.0f, 7.8f, 0.0f };

struct Particle
{
    float3 initialPosW : POSITION;
    float3 initialVelocityW : TEXCOORD0;
    float2 size : TEXCOORD1;
    float age : TEXCOORD2;
    uint type : TEXCOORD3;
};

struct VertexOut
{
    float3 posW : POSITION;
    float2 sizeW : SIZE;
    float4 Color : COLOR;
    uint type : TYPE;
};

Particle main(Particle vin)
{
    VertexOut vout;
    
    float t = vin.age;
    
    //constant acceleration equation
    vout.posW = 0.5f * t * t * accelW + t * vin.initialVelocityW + vin.initialPosW;
    
    //fade color with time
    float opacity = 1.0f - smoothstep(0.0f, 1.0f, t / 1.0f);
    vout.Color = float4(1.0f, 1.0f, 1.0, opacity);
    
    vout.sizeW = vin.size;
    vout.type = vin.type;
    
	return vout;
    
}