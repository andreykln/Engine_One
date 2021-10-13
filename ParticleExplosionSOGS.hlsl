#define PT_EMITTER 0
#define PT_FLARE 1
#define PT_SHELL 2
static const float shellLife = 0.3f;

cbuffer GSSObuffer : register(b0)
{
    float3 emitPosW;
    float timeStep;
    float gameTime;
    float3 padding;
}
   

float3 RandomUnitVec3(float offset);

struct Particle
{
    float3 initialPosW : POSITION;
    float3 initialVelocityW : TEXCOORD0;
    float2 size : TEXCOORD1;
    float age : TEXCOORD2;
    uint type : TEXCOORD3;
};


struct GSOutput
{
    float4 pos : SV_POSITION;
};

Texture1D randomTex : register(t0);
SamplerState samLinear : register(s0);

[maxvertexcount(4)]
void main(
	point Particle gin[1],
	inout PointStream<Particle> ptStream
)
{
    gin[0].age += timeStep;
    
    if (gin[0].type == PT_EMITTER)
    {
        for (int i = 0; i < 4; i++)
        {
            float3 vRandomDirection = RandomUnitVec3(0.0f);
                    
            Particle p;
            p.initialPosW = vRandomDirection;
            p.initialVelocityW = vRandomDirection;
            p.size = float2(1.0f, 1.0f);
            p.age = 0.0f;
            p.type = PT_SHELL;
        
            ptStream.Append(p);
            
        }
          //destroy only emitter
    }
    if(gin[0].type == PT_SHELL)
    {
        //time after emitter explosion, randomize?
        //shell creates flares, and disappears after
        if (gin[0].age > shellLife)
        {
            for (int j = 0; j < 3; j++)
            {
                float3 vRandomDirection = RandomUnitVec3(0.0f);
                    
                Particle p;
                p.initialPosW = vRandomDirection;
                p.initialVelocityW = vRandomDirection;
                p.size = float2(1.0f, 1.0f);
                p.age = 0.0f;
                p.type = PT_FLARE;
        
                ptStream.Append(p);
            }
        }
        //keep shells until they can emit
        if (gin[0].age < shellLife)
        {
            ptStream.Append(gin[0]);
        }
    }
    else
    {
		// Specify conditions to keep particle; this may vary from system to system.
        if (gin[0].age <= 1.0f)
            ptStream.Append(gin[0]);
    }
}


float3 RandomUnitVec3(float offset)
{
    float u = gameTime + offset;
    float3 v = randomTex.SampleLevel(samLinear, u, 0).xyz;
    
    return normalize(v);
}
