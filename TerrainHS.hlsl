float CalculateTessFactor(float3 p);
bool AabbBehindPlaneTest(float3 center, float3 extents, float4 plane);
bool AabbOutsideFrustumTest(float3 center, float3 extents, float4 frustumPlanes[6]);




cbuffer perFrame : register(b0)
{
   /*float minDist;
    float maxDist;
    float minTess;
    float maxTess;*/
	
    float4 worldFrustumPlanes[6];
    float3 camPosition;
    float padding;
}

struct VertexOut
{
    float3 posW : POSITION;
    float2 tex : TEXCOORD0;
    float2 boundsY : TEXCOORD1;
};

// Output control point
struct HS_CONTROL_POINT_OUTPUT
{
	float3 pos : WORLDPOS; 
    float2 tex : TEXCOORD0;
};

// Output patch constant data.
struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[4]			: SV_TessFactor; // e.g. would be [4] for a quad domain
	float InsideTessFactor[2]		: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
};

#define NUM_CONTROL_POINTS 4

// Patch Constant Function
HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<VertexOut, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT Output;

	//frustum culling
    //store the patch BoundsY in the first control point
    float minY = ip[0].boundsY.x;
    float maxY = ip[0].boundsY.y;
    
    //build axis-aligned bounding box. ip[2] is lower-left corner
    //and ip[1] is upper left corner
    float3 vMin = float3(ip[2].posW.x, minY, ip[2].posW.z);
    float3 vMax = float3(ip[1].posW.x, maxY, ip[1].posW.z);
    
    //center/extents representation 
    float3 boxCenter = 0.5f * (vMin + vMax);
    float3 boxExtents = 0.5f * (vMax - vMin);
    
    if (AabbOutsideFrustumTest(boxCenter, boxExtents, worldFrustumPlanes))
    {
        Output.EdgeTessFactor[0] = 0.0f;
        Output.EdgeTessFactor[1] = 0.0f;
        Output.EdgeTessFactor[2] = 0.0f;
        Output.EdgeTessFactor[3] = 0.0f;
		
        Output.InsideTessFactor[0] = 0.0f;
        Output.InsideTessFactor[1] = 0.0f;
        return Output;
    }
	
	
	//normal tesselation based on distance
    else
    {
		//compute midpoint on edges, and patch center
        float3 e0 = 0.5f * (ip[0].posW + ip[2].posW);
        float3 e1 = 0.5f * (ip[0].posW + ip[1].posW);
        float3 e2 = 0.5f * (ip[1].posW + ip[2].posW);
        float3 e3 = 0.5f * (ip[2].posW + ip[1].posW);
        float3 center = 0.25f * (ip[0].posW + ip[1].posW + ip[2].posW + ip[3].posW);
        Output.EdgeTessFactor[0] = CalculateTessFactor(e0);
        Output.EdgeTessFactor[1] = CalculateTessFactor(e1);
        Output.EdgeTessFactor[2] = CalculateTessFactor(e2);
        Output.EdgeTessFactor[3] = CalculateTessFactor(e3);
		
        Output.InsideTessFactor[0] = CalculateTessFactor(center);
        Output.InsideTessFactor[1] = Output.InsideTessFactor[0];
		
        return Output;
    }
}

[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("CalcHSPatchConstants")]
[maxtessfactor(64.0f)]
HS_CONTROL_POINT_OUTPUT main( 
	InputPatch<VertexOut, NUM_CONTROL_POINTS> ip,
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
	HS_CONTROL_POINT_OUTPUT Output;

	// Insert code to compute Output here
	Output.pos = ip[i].posW;
    Output.tex = ip[i].tex;

	return Output;
}


float CalculateTessFactor(float3 p)
{
    const float minDist = 5.0f;
    const float maxDist = 50.0f;
    const float minTess = 1.0f;
    const float maxTess = 64.0f;
    
    float d = distance(p, camPosition);
    float s = saturate((d - minDist) / (maxDist - minDist));
	
    return pow(2, lerp(maxTess, minTess, s));
}

    //return true if the box completely behind
    //(in negative half space) of plane
bool AabbBehindPlaneTest(float3 center, float3 extents, float4 plane)
{
    float3 n = abs(plane.xyz);
    
    //this is always positive
    float r = dot(extents, n);
    //signed distance from center point to plane
    float s = dot(float4(center, 1.0f), plane);
    
    //if the center point of the box is a distance of r or more
    //behind the plane (in which case s is negative since it is behind the plane)
    //then the box is completely in the negative half space of the plane
    
    return (r + s) < 0.0f;
}
bool AabbOutsideFrustumTest(float3 center, float3 extents, float4 frustumPlanes[6])
{
    for (int i = 0; i < 6; i++)
    {
        //if the box is completely behind any of the frustum planes
        //then it is outside the frustum
        if(AabbBehindPlaneTest(center, extents, frustumPlanes[i]))
        {
            return true;
        }
    }
    return false;

}