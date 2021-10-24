#define PT_EMITTER 0
#define PT_FLARE 1
#define PT_SHELL 2


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

[maxvertexcount(20)]
void main(
	point Particle gin[1],
	inout PointStream<Particle> ptStream
)
{
    gin[0].age += timeStep;
    float offset = 0.0f;

    //emits several shells at once then gets destroyed, not drawn
    if (gin[0].type == PT_EMITTER)
    {
        for (int i = 0; i < 1; i++)
        {
        
            float3 vRandomPosition = RandomUnitVec3(offset);
            offset += 1.0f;
            Particle p;
            p.initialPosW = vRandomPosition;
            p.initialVelocityW = 0.0f;
            p.size = float2(0.0f, 0.0f);
            p.age = 0.0f;
            p.type = PT_SHELL;
    
            ptStream.Append(p);
        }
    }
    //not drawn, emits flares
    if(gin[0].type == PT_SHELL)
    {
        //how long to keep shells so they emit flares
        if (gin[0].age < 0.3f)
        {
            ptStream.Append(gin[0]);
        }
        for (int j = 0; j < 1; j++)
        {
            
            float3 vRandomDirection = RandomUnitVec3(offset);
            offset += 1.0f;
            Particle p;
            p.initialPosW = gin[0].initialPosW;
            p.initialVelocityW = vRandomDirection;
            p.size = float2(0.3f, 0.3f);
            p.age = 0.0f;
            p.type = PT_FLARE;
            ptStream.Append(p);
            
        }
   

    }
    else
    {
        //how long to keep flares
        if (gin[0].age < 0.4f)
            ptStream.Append(gin[0]);
    }
}


float3 RandomUnitVec3(float offset)
{
    float u = gameTime + offset;
    float3 v = randomTex.SampleLevel(samLinear, u, 0).xyz;
    
    return normalize(v);
}
