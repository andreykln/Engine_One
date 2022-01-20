float CalculateTessFactor(float3 p);
bool AabbBehindPlaneTest(float3 center, float3 extents, float4 plane);
bool AabbOutsideFrustumTest(float3 center, float3 extents, float4 frustumPlanes[6]);






struct VertexOut
{
    float3 posW : POSITION;
    float2 tex : TEXCOORD0;
    float2 boundsY : TEXCOORD1;
};





float CalculateTessFactor(float3 p)
{
    const float minDist = 20.0f;
    const float maxDist = 500.0f;
    const float minTess = 0.0f;
    const float maxTess = 6.0f;
    
    float d = distance(p, camPosition);
    float s = saturate((d - minDist) / (maxDist - minDist));
	
    return pow(2, lerp(maxTess, minTess, s));
}

    //return true if the box completely behind
    //(in negative half space) of plane
bool AabbBehindPlaneTest(float3 center, float3 extents, float4 plane)
{
    float3 n = abs(plane.xyz); //(|n.x|, |n.y|, |n.z|)
    //Why this is a plane normal? And I don't understand how a plane can be defined with x,y,z,w
    
    //this is always positive
    float r = dot(extents, n);
    //This is the projection on plane's normal, right? So it's the vector from the plane
    //to the farthest edge of the box
    
    //signed distance from center point to plane
    float s = dot(float4(center, 1.0f), plane);
    //This one I don't understand at all. Why dot product is a signed distance? 
    
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