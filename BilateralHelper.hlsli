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
float3 LABcolor(float r, float g, float b)
{
    //RGB to XYZ
    float X, Y, Z;
    float var_R, var_G, var_B;
    if (r > 0.04045)
    {
        var_R = pow(((r + 0.055) / 1.055), 2.4f);
    }
    else
    {
        var_R = r / 12.92f;
    }
    if (g > 0.04045)
    {
        var_G = pow(((g + 0.055) / 1.055), 2.4f);
    }
    else
    {
        var_G = g / 12.92f;
    }
    if (b > 0.04045)
    {
        var_B = pow(((b + 0.055) / 1.055), 2.4f);
    }
    else
    {
        var_B = b / 12.92f;
    }
    
    var_R = var_R * 100.0f;
    var_G = var_G * 100.0f;
    var_B = var_B * 100.0f;
    X = var_R * 0.4124 + var_G * 0.3576 + var_B * 0.1805;
    Y = var_R * 0.2126 + var_G * 0.7152 + var_B * 0.0722;
    Z = var_R * 0.0193 + var_G * 0.1192 + var_B * 0.9505;

    //XYZ to Lab
    float reference_X = 100.0f;
    float reference_Y = 100.0f;
    float reference_Z = 100.0f;
    X = X / reference_X;
    Y = Y / reference_Y;
    Z = Z / reference_Z;
    float addition = 16 / 116;
    if (X > 0.008856)
    {
        X = pow(X, 0.333333f);
    }
    else
    {
        X = (7.787 * X) + addition;
    }
    if (Y > 0.008856)
    {
        Y = pow(Y, 0.333333f);
    }
    else
    {
        Y = (7.787 * Y) + addition;
    }
    if (Z > 0.008856)
    {
        Z = pow(Z, 0.333333f);
    }
    else
    {
        Z = (7.787 * Z) + addition;
    }
    
    float L, A, B;
    L = (116 * Y) - 16;
    A = 500 * (X - Y);
    B = 200 * (Y - Z);
    float3 Lab = float3(L, A, B);
    return Lab;
}