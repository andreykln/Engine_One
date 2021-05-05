static const int blurRadius = 5u;
static const int nThreads = 256u;
static const int cacheSize = (nThreads + 2 * blurRadius);
groupshared float4 cache[cacheSize];

Texture2D Input : register(t0);
Texture2D test : register(t1);
RWTexture2D<float4> Output : register(u0);


/*struct Weights
{
    float weight[11] =
    {
        0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f,
    };
};*/

[numthreads(nThreads, 1, 1)]
void main( int3 groupThreadID : SV_GroupThreadID, int3 dispathThreadID : SV_DispatchThreadID )
{
    uint width;
    uint heigth;
    uint numOfLevels;
    Input.GetDimensions(0u, width, heigth, numOfLevels);
    
    
    float Weights[11] =
    {
        0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f,
    };
 
    if(groupThreadID.x < blurRadius)
    {
        //clamp out of bound samples that occer at image borders
        int x = max(dispathThreadID.x - blurRadius, 0);
        cache[groupThreadID.x] = Input[int2(x, dispathThreadID.y)];
    }
    if(groupThreadID.x >= nThreads - blurRadius)
    {
        int x = min(dispathThreadID.x + blurRadius, (width - 1));
        cache[groupThreadID.x + 2 * blurRadius] = Input[int2(x, dispathThreadID.y)];
    }
    cache[groupThreadID.x + blurRadius] = Input[dispathThreadID.xy];
    
    GroupMemoryBarrierWithGroupSync();
    
    float4 blurColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    for (int i = -blurRadius; i <= blurRadius; ++i)
    {
        int k = groupThreadID.x + blurRadius + i;
        blurColor += Weights[i + blurRadius] * cache[k];
    }
    Output[dispathThreadID.xy] = blurColor;
}

