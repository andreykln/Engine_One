struct VertexOut
{
    float4 posH : SV_Position;
    float2 texC : TEXCOORD;
};

const static float weights[11] =
{
    0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f
};

static const int blurRadius = 5;

cbuffer blurCB : register(b0)
{
    float texelWidth;
    float texelHeight;
    bool horizBool;
    int pad0;
};

Texture2D normalDepthMap : register(t0);
Texture2D inputImage : register(t1);
SamplerState samplerBlur : register(s2);

float4 main(VertexOut pin) : SV_TARGET
{
    float2 texOffset;
    if(horizBool)
    {
        texOffset = float2(texelHeight, 0.0f);
    }
    else
    {
        texOffset = float2(0.0f, texelHeight);
    }
    // The center value always contributes to the sum.
    float4 color = weights[5] * inputImage.SampleLevel(samplerBlur, pin.texC, 0.0);
    float totalWeight = weights[5];
    
    float4 centerNormalDepth = normalDepthMap.SampleLevel(samplerBlur, pin.texC, 0.0f);
    for (float i = -blurRadius; i <= blurRadius; ++i)
    {
		// We already added in the center weight.
        if (i == 0)
            continue;

        float2 tex = pin.texC + i * texOffset;

        float4 neighborNormalDepth = normalDepthMap.SampleLevel(
			samplerBlur, tex, 0.0f);

		//
		// If the center value and neighbor values differ too much (either in 
		// normal or depth), then we assume we are sampling across a discontinuity.
		// We discard such samples from the blur.
		//
	
        if (dot(neighborNormalDepth.xyz, centerNormalDepth.xyz) >= 0.8f &&
		    abs(neighborNormalDepth.a - centerNormalDepth.a) <= 0.2f)
        {
            float weight = weights[i + blurRadius];

			// Add neighbor pixel to blur.
            color += weight * inputImage.SampleLevel(
				samplerBlur, tex, 0.0);
		
            totalWeight += weight;
        }
    }
    
	// Compensate for discarded samples by making total weights sum to 1.
    return color / totalWeight;
}