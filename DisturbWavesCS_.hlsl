cbuffer cbUpdateSettings
{
    float waveConstant0;
    float waveConstant1;
    float waveConstant2;
	
    float disturbMagnitude;
    int2 disturbIndex;
    int2 padding;
};

RWTexture2D<float> Output : register(u0);

[numthreads(1, 1, 1)]
void main(int3 groupThreadID : SV_GroupThreadID,
          int3 dispatchThreadID : SV_DispatchThreadID)
{
    
    int x = disturbIndex.x;
    int y = disturbIndex.y;

    float halfMag = 0.5f * disturbMagnitude;

	// Buffer is RW so operator += is well defined.
    Output[int2(x, y)] += disturbMagnitude;
    Output[int2(x + 1, y)] += halfMag;
    Output[int2(x - 1, y)] += halfMag;
    Output[int2(x, y + 1)] += halfMag;
    Output[int2(x, y - 1)] += halfMag;
}