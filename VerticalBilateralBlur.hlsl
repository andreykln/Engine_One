#include "BilateralHelper.hlsli"
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
    //color of 
    //calculate intensity weights
    float x = Input[groupThreadID.xy].x;
    float y = Input[groupThreadID.xy].y;
    float z = Input[groupThreadID.xy].z;
    float3 centerRGBcolor = float3(x, y, z);
    //float3 centerLabColor = LABcolor(x, y, z);
    float normalizationCoefficient = 0.0f;
    
    for (int i = 0; i < numOfIntensityWeights; ++i)
    {
        float x0 = Input[int2(groupThreadID.x - blurRadius + i, groupThreadID.y)].x;
        float y0 = Input[int2(groupThreadID.x - blurRadius + i, groupThreadID.y)].y;
        float z0 = Input[int2(groupThreadID.x - blurRadius + i, groupThreadID.y)].z;
        //float3 sampledLabColor = LABcolor(x0, y0, z0);
        float3 sampledRGBcolor = float3(x0, y0, z0);
        //float intensityOfSampledPixelLab = sampledLabColor.x;
        float intensityRGB = CalcLuminance(sampledRGBcolor);
        float LABlength = GaussianFunction1D(length(centerRGBcolor - sampledRGBcolor));
        intensityWeights[i] = LABlength * intensityRGB * rangeWeights[i];
        normalizationCoefficient += LABlength * rangeWeights[i];
    }
    
    for (int j = 0; j < numOfIntensityWeights; ++j)
    {
        intensityWeights[j] = (intensityWeights[j] / normalizationCoefficient);
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
    Output[dispathThreadID.xy] = normalize(blurColor);

}

