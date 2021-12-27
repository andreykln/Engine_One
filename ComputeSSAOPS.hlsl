cbuffer SSAOConstBuffer : register(b0)
{
    float4x4 viewToTexSpace;
    float4 offsetVectors[14];
    float4 frustumFarCorners[4];

    const float gOcclusionRadius;
    const float gOcclusionFadeStart;
    const float gOcclusionFadeEnd;
    const float gSurfaceEpsilon;
};

SamplerState normalMapSmp : register(s0);
SamplerState randomVecSmp : register(s1);

Texture2D randomVectorSRV : register(t0);
Texture2D normalMapSRV : register(t1);

static const int sampleCount = 14;

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 ToFarPlane : TEXCOORD0;
    float2 Tex : TEXCOORD1;
};
float OcclusionFunction(float distZ)
{
	//
	// If depth(q) is "behind" depth(p), then q cannot occlude p.  Moreover, if 
	// depth(q) and depth(p) are sufficiently close, then we also assume q cannot
	// occlude p because q needs to be in front of p by Epsilon to occlude p.
	//
	// We use the following function to determine the occlusion.  
	// 
	//
	//       1.0     -------------\
	//               |           |  \
	//               |           |    \
	//               |           |      \ 
	//               |           |        \
	//               |           |          \
	//               |           |            \
	//  ------|------|-----------|-------------|---------|--> zv
	//        0     Eps          z0            z1        
	//
	
    float occlusion = 0.0f;
    if (distZ > gSurfaceEpsilon)
    {
        float fadeLength = gOcclusionFadeEnd - gOcclusionFadeStart;
		
		// Linearly decrease occlusion from 1 to 0 as distZ goes 
		// from gOcclusionFadeStart to gOcclusionFadeEnd.	
        occlusion = saturate((gOcclusionFadeEnd - distZ) / fadeLength);
    }
	
    return occlusion;
}
float4 main(VertexOut pin) : SV_Target
{
    // p -- the point we are computing the ambient occlusion for.
	// n -- normal vector at p.
	// q -- a random offset from p.
	// r -- a potential occluder that might occlude p.

	// Get viewspace normal and z-coord of this pixel.  The tex-coords for
	// the fullscreen quad we drew are already in uv-space.
    float4 normalDepth = normalMapSRV.SampleLevel(normalMapSmp, pin.Tex, 0.0f);
	
    float3 n = normalDepth.xyz;
    float pz = normalDepth.w;
	//
	// Reconstruct full view space position (x,y,z).
	
	//pin.ToFarPlane is interpolated?
	//vector to the p point
	// Find t such that p = t*pin.ToFarPlane.
	// p.z = t*pin.ToFarPlane.z
	// t = p.z / pin.ToFarPlane.z
	//
    float3 p = (pz / pin.ToFarPlane.z) * pin.ToFarPlane;
	
	// Extract random vector and map from [0,1] --> [-1, +1].
    float3 randVec = 2.0f * randomVectorSRV.SampleLevel(randomVecSmp, 4.0f * pin.Tex, 0.0f).rgb - 1.0f;

    float occlusionSum = 0.0f;

	[unroll]
    for (int i = 0; i < sampleCount; i++)
    {
		// All offset vectors are fixed and uniformly distributed  across verticxes of a cube
		// (so that our offset vectors do not clump in the same direction).
		// If we reflect them about a random vector
		// then we get a random uniform distribution of offset vectors.
        float3 offset = reflect(offsetVectors[i].xyz, randVec);
		// Flip offset vector if it is behind the plane defined by (p, n).
        float flip = sign(dot(offset, n));
		// Sample a point near p within the occlusion radius.
        float3 q = p + flip * gOcclusionRadius * offset;
		// Project q and generate projective tex-coords.  
        float4 projQ = mul(float4(q, 1.0f), viewToTexSpace);
        projQ /= projQ.w;
		
		// Find the nearest depth value along the ray from the eye to q (this is not
		// the depth of q, as q is just an arbitrary point near p and might
		// occupy empty space).  To find the nearest depth we look it up in the depthmap.
		
		//found a sample point, project it to the texture space, find viewSpace depth at the same xy coorditanes
		//of a closest point in normal-depth map
        float rz = normalMapSRV.SampleLevel(normalMapSmp, projQ.xy, 0.0f).a;
		
		// Reconstruct full view space position r = (rx,ry,rz).  We know r
		// lies on the ray of q, so there exists a t such that r = t*q.
		// r.z = t*q.z ==> t = r.z / q.z
        float3 r = (rz / q.z) * q;
		
		// Test whether r occludes p.
		//   * The product dot(n, normalize(r - p)) measures how much in front
		//     of the plane(p,n) the occluder point r is.  The more in front it is, the
		//     more occlusion weight we give it.  This also prevents self shadowing where 
		//     a point r on an angled plane (p,n) could give a false occlusion since they
		//     have different depth values with respect to the eye.
		//   * The weight of the occlusion is scaled based on how far the occluder is from
		//     the point we are computing the occlusion of.  If the occluder r is far away
		//     from p, then it does not occlude it.
		// 
		
        float distZ = p.z - r.z;
        float dp = max(dot(n, normalize(r - p)), 0.0f);
        float occlusion = dp * OcclusionFunction(distZ);
		
        occlusionSum += occlusion;
    }
    occlusionSum /= sampleCount;
	
    float access = 1.0f - occlusionSum;

	// Sharpen the contrast of the SSAO map to make the SSAO affect more dramatic.
    return saturate(pow(access, 4.0f));
	
}