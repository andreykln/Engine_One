static const float PI = 3.14159265f;

float GaussianFunction1D(float intensityDiff)
{
    float sigma = 3.0f;
    float exponent = exp((-1.0f * intensityDiff * intensityDiff) / (2 * sigma * sigma));
    float normalization = exponent / ((2 * PI) * sigma * sigma);
    return normalization;
}

float CalcLuminance(float3 color)
{
    return dot(color, float3(0.299f, 0.587f, 0.114f));
}
