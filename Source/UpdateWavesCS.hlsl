cbuffer cbUpdateSettings
{
    float waveConstant0;
    float waveConstant1;
    float waveConstant2;
	
    float disturbMagnitude;
    int2 disturbIndex;
};
 
Texture2D PreviousSolutionInput : register(t0);
Texture2D CurrentSolutionInput : register(t1);
RWTexture2D<float> nextSolutionOutput : register(u0);

[numthreads(16, 16, 1)]
void main(int3 dispatchThreadID : SV_DispatchThreadID)
{
    
    int x = dispatchThreadID.x;
    int y = dispatchThreadID.y;

    nextSolutionOutput[int2(x, y)] =
		waveConstant0 * PreviousSolutionInput[int2(x, y)].r +
		waveConstant1 * CurrentSolutionInput[int2(x, y)].r +
		waveConstant2 * (
			CurrentSolutionInput[int2(x, y + 1)].r +
			CurrentSolutionInput[int2(x, y - 1)].r +
			CurrentSolutionInput[int2(x + 1, y)].r +
			CurrentSolutionInput[int2(x - 1, y)].r);

}