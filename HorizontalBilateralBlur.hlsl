static const int blurRadius = 5u;
static const int nThreads = 256u;
static const int cacheSize = (nThreads + 2 * blurRadius);
static const int numOfIntensityWeights = 11;
groupshared float4 cache[cacheSize];

Texture2D Input : register(t0);
RWTexture2D<float4> Output : register(u0);
SamplerState texSample : register(s0);


[numthreads(nThreads, 1, 1)]
void main(int3 groupThreadID : SV_GroupThreadID, int3 dispathThreadID : SV_DispatchThreadID)
{
    uint width;
    uint heigth;
    uint numOfLevels;
    Input.GetDimensions(0u, width, heigth, numOfLevels);
    
    float intensityWeights[numOfIntensityWeights];
    const float rangeWeights[11] =
    {
        0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f,
    };

    //calculate intensity weights

    float x = Input[dispathThreadID.xy].x;
    float y = Input[dispathThreadID.xy].y;
    float z = Input[dispathThreadID.xy].z;
    float3 center = float3(x, y, z);
    float normalizationCoefficient = 0.0f;
    
    for (int i = 0; i < numOfIntensityWeights; ++i)
    {
        float x = Input[int2(dispathThreadID.x - blurRadius + i, dispathThreadID.y)].x;
        float y = Input[int2(dispathThreadID.x - blurRadius + i, dispathThreadID.y)].y;
        float z = Input[int2(dispathThreadID.x - blurRadius + i, dispathThreadID.y)].z;
        float3 color = float3(x, y, z);
        float RGBlength = length(color - center);
        intensityWeights[i] = RGBlength;
        normalizationCoefficient += intensityWeights[i];
    }
    for (int j = 0; j < numOfIntensityWeights; ++j)
    {
        intensityWeights[j] = intensityWeights[j] / normalizationCoefficient;

    }
    //weights which occur outside of the image borders is set to 1 to they have no effect
    //left image border
    if (groupThreadID.x < blurRadius)
    {
  
        for (int n = 0; n < blurRadius; ++n)
        {
            intensityWeights[n] = 1.0f;
        }
    }
    //right image border
    if (groupThreadID.x >= nThreads - blurRadius)
    {
        for (int q = numOfIntensityWeights - blurRadius; q < numOfIntensityWeights; ++q)
        {
            intensityWeights[q] = 1.0f;
        }
    }
 
   if (groupThreadID.x < blurRadius)
    {
        //clamp out of bound samples that occer at image borders
        int x = max(dispathThreadID.x - blurRadius, 0);
        cache[groupThreadID.x] = Input[int2(x, dispathThreadID.y)];
    }
    if (groupThreadID.x >= nThreads - blurRadius)
    {
        int x = min(dispathThreadID.x + blurRadius, (width - 1));
        cache[groupThreadID.x + 2 * blurRadius] = Input[int2(x, dispathThreadID.y)];
    }
    cache[groupThreadID.x + blurRadius] = Input[dispathThreadID.xy];
    
    GroupMemoryBarrierWithGroupSync();
    
    float4 blurColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    for (int m = -blurRadius; m <= blurRadius; ++m)
    {
        int k = groupThreadID.x + blurRadius + m;
        blurColor += rangeWeights[m + blurRadius] * cache[k] * intensityWeights[m + blurRadius];
    }
    Output[dispathThreadID.xy] = blurColor;
}

