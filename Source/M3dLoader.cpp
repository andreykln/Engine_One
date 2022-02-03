#include "M3dLoader.h"

M3dLoader::M3dLoader(const std::string& filename, bool skinned)
{
	if (skinned)
	{
		bool loaded = LoadM3d(filename, rawSkinnedData.vertices, rawSkinnedData.indices, rawSkinnedData.subsets,
			rawSkinnedData.mats, rawSkinnedData.skinnedInfo);
	}
	else
	{
		bool loaded = LoadM3d(filename, rawData.vertices, rawData.indices, rawData.subsets, rawData.mats);
	}

}

bool M3dLoader::LoadM3d(const std::string& filename,
	std::vector<vbPosNormalTexTangent>& vertices,
	std::vector<UINT>& indices,
	std::vector<Subset>& subsets,
	std::vector<M3dMaterial>& mats)
{
	std::ifstream fin(filename);

	UINT numMaterials = 0;
	UINT numVertices = 0;
	UINT numTriangles = 0;
	UINT numBones = 0;
	UINT numAnimationClips = 0;

	std::string ignore;

	if (fin)
	{
		fin >> ignore; //file header
		fin >> ignore >> numMaterials;
		fin >> ignore >> numVertices;
		fin >> ignore >> numTriangles;
		fin >> ignore >> numBones;
		fin >> ignore >> numAnimationClips;
		ReadMaterials(fin, numMaterials, mats);
		ReadSubsetTable(fin, numMaterials, subsets);
		ReadVertices(fin, numVertices, vertices);
		ReadTriangles(fin, numTriangles, indices);
		return true;
	}
	return false;

}

bool M3dLoader::LoadM3d(const std::string& filename,
	std::vector<vbSkinnedVertex>& vertices,
	std::vector<UINT>& indices,
	std::vector<Subset>& subsets,
	std::vector<M3dMaterial>& mats,
	SkinnedData& skinInfo)
{
	std::ifstream fin(filename);

	UINT numMaterials = 0;
	UINT numVertices = 0;
	UINT numTriangles = 0;
	UINT numBones = 0;
	UINT numAnimationClips = 0;

	std::string ignore;

	if (fin)
	{
		fin >> ignore; // file header text
		fin >> ignore >> numMaterials;
		fin >> ignore >> numVertices;
		fin >> ignore >> numTriangles;
		fin >> ignore >> numBones;
		fin >> ignore >> numAnimationClips;

		std::vector<DirectX::XMFLOAT4X4> boneOffsets;
		std::vector<int> boneIndexToParentIndex;
		std::unordered_map<std::string, AnimationClip> animations;

		ReadMaterials(fin, numMaterials, mats);
		ReadSubsetTable(fin, numMaterials, subsets);
		ReadSkinnedVertices(fin, numVertices, vertices);
		ReadTriangles(fin, numTriangles, indices);
		ReadBoneOffsets(fin, numBones, boneOffsets);
		ReadBoneHierarchy(fin, numBones, boneIndexToParentIndex);
		ReadAnimationClips(fin, numBones, numAnimationClips, animations);

		skinInfo.Set(boneIndexToParentIndex, boneOffsets, animations);

		return true;
	}
	return false;
}

std::wstring M3dLoader::utf8toUtf16(const std::string& str)
{
	
	if (str.empty())
		return std::wstring();

	size_t charsNeeded = ::MultiByteToWideChar(CP_UTF8, 0,
		str.data(), (int)str.size(), NULL, 0);

	std::vector<wchar_t> buffer(charsNeeded);
	int charsConverted = ::MultiByteToWideChar(CP_UTF8, 0,
		str.data(), (int)str.size(), &buffer[0], (int)buffer.size());

	return std::wstring(&buffer[0], charsConverted);
	
}

void M3dLoader::ReadMaterials(std::ifstream& fin, UINT numMaterials, std::vector<M3dMaterial>& mats)
{
	std::string ignore;
	mats.resize(numMaterials);

	std::wstring diffuseMapName;
	std::wstring normalMapName;

	fin >> ignore; //material header text
	for (UINT i = 0; i < numMaterials; i++)
	{
		std::string tempName;
		fin >> ignore >> mats[i].name;
		fin >> ignore >> mats[i].diffuseAlbedo.x >> mats[i].diffuseAlbedo.y >> mats[i].diffuseAlbedo.z;
		fin >> ignore >> mats[i].fresnelR0.x >> mats[i].fresnelR0.y >> mats[i].fresnelR0.z;
		fin >> ignore >> mats[i].shininess;
		fin >> ignore >> mats[i].alphaClip;
		fin >> ignore >> mats[i].materialTypeName;
		fin >> ignore >> tempName;
		mats[i].diffuseMapName = utf8toUtf16(tempName);
		tempName.clear();
		fin >> ignore >> tempName;
		mats[i].normalMapName = utf8toUtf16(tempName);
		tempName.clear();
	}
}

void M3dLoader::ReadSubsetTable(std::ifstream& fin, UINT numSubsets, std::vector<Subset>& subsets)
{
	std::string ignore;
	subsets.resize(numSubsets);

	fin >> ignore;
	for (UINT i = 0; i < numSubsets; i++)
	{
		fin >> ignore >> subsets[i].ID;
		fin >> ignore >> subsets[i].VertexStart;
		fin >> ignore >> subsets[i].VertexCount;
		fin >> ignore >> subsets[i].FaceStart;
		fin >> ignore >> subsets[i].FaceCount;
	}
}

void M3dLoader::ReadVertices(std::ifstream& fin, UINT numVertices, std::vector<vbPosNormalTexTangent>& vertices)
{
	vertices.resize(numVertices);
	std::string ignore;

	fin >> ignore;
	for (UINT i = 0; i < numVertices; i++)
	{
		float dummy;
		fin >> ignore >> vertices[i].pos.x >> vertices[i].pos.y >> vertices[i].pos.z;
		fin >> ignore >> vertices[i].tangent.x >> vertices[i].tangent.y >> vertices[i].tangent.z >> dummy; //tangen.w?
		fin >> ignore >> vertices[i].normal.x >> vertices[i].normal.y >> vertices[i].normal.z;
		fin >> ignore >> vertices[i].tex.x >> vertices[i].tex.y;
	}
}

void M3dLoader::ReadTriangles(std::ifstream& fin, UINT numTriangles, std::vector<UINT>& indices)
{
	indices.resize(numTriangles * 3);
	std::string ignore;

	fin >> ignore;
	for (UINT i = 0; i < numTriangles; i++)
	{
		fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}

}

void M3dLoader::ReadSkinnedVertices(std::ifstream& fin, UINT numVertices, std::vector<vbSkinnedVertex>& vertices)
{
	std::string ignore;
	vertices.resize(numVertices);

	fin >> ignore; // vertices header text
	int boneIndices[4];
	float weights[4];
	for (UINT i = 0; i < numVertices; ++i)
	{
		float blah;
		fin >> ignore >> vertices[i].pos.x >> vertices[i].pos.y >> vertices[i].pos.z;
		fin >> ignore >> vertices[i].tangent.x >> vertices[i].tangent.y >> vertices[i].tangent.z >> blah /*vertices[i].TangentU.w*/;
		fin >> ignore >> vertices[i].normal.x >> vertices[i].normal.y >> vertices[i].normal.z;
		fin >> ignore >> vertices[i].tex.x >> vertices[i].tex.y;
		fin >> ignore >> weights[0] >> weights[1] >> weights[2] >> weights[3];
		fin >> ignore >> boneIndices[0] >> boneIndices[1] >> boneIndices[2] >> boneIndices[3];

		vertices[i].BoneWeights.x = weights[0];
		vertices[i].BoneWeights.y = weights[1];
		vertices[i].BoneWeights.z = weights[2];

		vertices[i].BoneIndices[0] = (BYTE)boneIndices[0];
		vertices[i].BoneIndices[1] = (BYTE)boneIndices[1];
		vertices[i].BoneIndices[2] = (BYTE)boneIndices[2];
		vertices[i].BoneIndices[3] = (BYTE)boneIndices[3];
	}
}

void M3dLoader::ReadBoneOffsets(std::ifstream& fin, UINT numBones, std::vector<DirectX::XMFLOAT4X4>& boneOffsets)
{
	std::string ignore;
	boneOffsets.resize(numBones);

	fin >> ignore; // BoneOffsets header text
	for (UINT i = 0; i < numBones; ++i)
	{
		fin >> ignore >>
			boneOffsets[i](0, 0) >> boneOffsets[i](0, 1) >> boneOffsets[i](0, 2) >> boneOffsets[i](0, 3) >>
			boneOffsets[i](1, 0) >> boneOffsets[i](1, 1) >> boneOffsets[i](1, 2) >> boneOffsets[i](1, 3) >>
			boneOffsets[i](2, 0) >> boneOffsets[i](2, 1) >> boneOffsets[i](2, 2) >> boneOffsets[i](2, 3) >>
			boneOffsets[i](3, 0) >> boneOffsets[i](3, 1) >> boneOffsets[i](3, 2) >> boneOffsets[i](3, 3);
	}
}

void M3dLoader::ReadBoneHierarchy(std::ifstream& fin, UINT numBones, std::vector<int>& boneIndexToParentIndex)
{
	std::string ignore;
	boneIndexToParentIndex.resize(numBones);

	fin >> ignore; // BoneHierarchy header text
	for (UINT i = 0; i < numBones; ++i)
	{
		fin >> ignore >> boneIndexToParentIndex[i];
	}
}

void M3dLoader::ReadAnimationClips(std::ifstream& fin,
	UINT numBones,
	UINT numAnimationClips,
	std::unordered_map<std::string, AnimationClip>& animations)
{
	std::string ignore;
	fin >> ignore; // AnimationClips header text
	for (UINT clipIndex = 0; clipIndex < numAnimationClips; ++clipIndex)
	{
		std::string clipName;
		fin >> ignore >> clipName;
		fin >> ignore;

		AnimationClip clip;
		clip.BoneAnimations.resize(numBones);

		for (UINT boneIndex = 0; boneIndex < numBones; ++boneIndex)
		{
			ReadBoneKeyframes(fin, numBones, clip.BoneAnimations[boneIndex]);
		}
		fin >> ignore; 

		animations[clipName] = clip;
	}
}

void M3dLoader::ReadBoneKeyframes(std::ifstream& fin, UINT numBones, BoneAnimation& boneAnimation)
{
	std::string ignore;
	UINT numKeyframes = 0;
	fin >> ignore >> ignore >> numKeyframes;
	fin >> ignore; 

	boneAnimation.keyframes.resize(numKeyframes);
	for (UINT i = 0; i < numKeyframes; ++i)
	{
		float t = 0.0f;
		DirectX::XMFLOAT3 p(0.0f, 0.0f, 0.0f);
		DirectX::XMFLOAT3 s(1.0f, 1.0f, 1.0f);
		DirectX::XMFLOAT4 q(0.0f, 0.0f, 0.0f, 1.0f);
		fin >> ignore >> t;
		fin >> ignore >> p.x >> p.y >> p.z;
		fin >> ignore >> s.x >> s.y >> s.z;
		fin >> ignore >> q.x >> q.y >> q.z >> q.w;

		boneAnimation.keyframes[i].timePos = t;
		boneAnimation.keyframes[i].translation = p;
		boneAnimation.keyframes[i].scale = s;
		boneAnimation.keyframes[i].rotationQuat = q;
	}
	fin >> ignore;
}
