#define PT_EMITTER 0
#define PT_FLARE 1

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

[maxvertexcount(6)]
void main(
	point Particle gin[1],
	inout PointStream<Particle> ptStream
)
{
    gin[0].age += timeStep;
    float offset = 0.0f;
    if (gin[0].type == PT_EMITTER)
    {
        //time to emit new particle?
        if (gin[0].age > 0.002f)
        {
            for (int i = 0; i < 5; i++)
            {
                float3 vRandom = RandomUnitVec3(offset);
                offset += 0.01f;
               // vRandom.y = 20.0f;
                            
                Particle p;
                p.initialPosW = emitPosW.xyz;
                p.initialVelocityW = vRandom;
                p.size = float2(1.0f, 1.0f);
                p.age = 0.0f;
                p.type = PT_FLARE;
            
                ptStream.Append(p);
            }

            // reset the time to emit
            gin[0].age = 0.0f;
        }
      
        // always keep emitters
        ptStream.Append(gin[0]);
    }
    else
    {
		// Specify conditions to keep particle; this may vary from system to system.
        if (gin[0].age <= 3.0f)
            ptStream.Append(gin[0]);
    }
}


float3 RandomUnitVec3(float offset)
{
    float u = gameTime + offset;
    float3 v = randomTex.SampleLevel(samLinear, u, 0).xyz;
    
    return normalize(v);
}
