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

Texture2D normalMapSRV : register(t3);
Texture2D inputImage : register(t5);

//random vector map
SamplerState smpLinearWrap : register(s0);
SamplerState smpAnisotropicWrap : register(s1);
SamplerComparisonState smpShadowMap : register(s2);
//only for normal depth map
SamplerState smpNormalMap : register(s3);
//blur map
SamplerState smpLinearClamp : register(s4);

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
    float4 color = weights[5] * inputImage.SampleLevel(smpLinearClamp, pin.texC, 0.0);
    float totalWeight = weights[5];
    
    float4 centerNormalDepth = normalMapSRV.SampleLevel(smpLinearClamp, pin.texC, 0.0f);
    for (float i = -blurRadius; i <= blurRadius; ++i)
    {
		// We already added in the center weight.
        if (i == 0)
            continue;

        float2 tex = pin.texC + i * texOffset;

        float4 neighborNormalDepth = normalMapSRV.SampleLevel(
			smpLinearClamp, tex, 0.0f);

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
				smpLinearClamp, tex, 0.0);
		
            totalWeight += weight;
        }
    }
    
	// Compensate for discarded samples by making total weights sum to 1.
    return color / totalWeight;
}