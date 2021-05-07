static const int blurRadius = 5u;
static const int nThreads = 256u;
static const int cacheSize = (nThreads + 2 * blurRadius);
static const int numOfIntensityWeights = 11;
groupshared float4 cache[cacheSize];

Texture2D Input : register(t0);
RWTexture2D<float4> Output : register(u0);
SamplerState texSample : register(s0);


[numthreads(1, nThreads, 1)]
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
        float x = Input[int2(dispathThreadID.x, dispathThreadID.y - blurRadius + i)].x;
        float y = Input[int2(dispathThreadID.x, dispathThreadID.y - blurRadius + i)].y;
        float z = Input[int2(dispathThreadID.x, dispathThreadID.y - blurRadius + i)].z;
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
    if (groupThreadID.y < blurRadius)
    {
  
        for (int n = 0; n < blurRadius; ++n)
        {
            intensityWeights[n] = 1.0f;
        }
    }
    //right image border
    if (groupThreadID.y >= nThreads - blurRadius)
    {
        for (int q = numOfIntensityWeights - blurRadius; q < numOfIntensityWeights; ++q)
        {
            intensityWeights[q] = 1.0f;
        }
    }
 
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
    
    for (int m = -blurRadius; m <= blurRadius; ++m)
    {
        int k = groupThreadID.y + blurRadius + m;
        blurColor += rangeWeights[m + blurRadius] * cache[k] * intensityWeights[m + blurRadius];
    }
    Output[dispathThreadID.xy] = blurColor;

}

