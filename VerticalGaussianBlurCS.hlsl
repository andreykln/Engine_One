static const int blurRadius = 5u;
static const int nThreads = 256u;
static const int cacheSize = (nThreads + 2 * blurRadius);
groupshared float4 cache[cacheSize];

Texture2D Input : register(t0);
RWTexture2D<float4> Output : register(u0);

[numthreads(1, nThreads, 1)]
void main(int3 groupThreadID : SV_GroupThreadID, int3 dispathThreadID : SV_DispatchThreadID)
{
    uint width;
    uint heigth;
    uint numOfLevels;
    Input.GetDimensions(0u, width, heigth, numOfLevels);
    
    const float Weights[11] =
    {
        0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f,
    };
    if (groupThreadID.y < blurRadius)
    {
        //clamp out of bound samples that occer at image borders
        int y = max(dispathThreadID.y - blurRadius, 0);
        cache[groupThreadID.y] = Input[int2(dispathThreadID.x, y)];
    }
    if (groupThreadID.y >= nThreads - blurRadius)
    {
        int y = min(dispathThreadID.y + blurRadius, (heigth - 1));
        cache[groupThreadID.y + 2 * blurRadius] = Input[int2(dispathThreadID.x, y)];
    }
    cache[groupThreadID.y + blurRadius] = Input[dispathThreadID.xy];
    
    GroupMemoryBarrierWithGroupSync();
    
    
    float4 blurColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    for (int i = -blurRadius; i <= blurRadius; ++i)
    {
        int k = groupThreadID.y + blurRadius + i;
        blurColor += Weights[i + blurRadius] * cache[k];
    }
    Output[dispathThreadID.xy] = blurColor;
    
}