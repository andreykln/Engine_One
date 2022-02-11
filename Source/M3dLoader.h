#pragma once
#include <fstream>
#include <vector>
//for WinBase.h in Utility
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags 

#include <windows.h>
#include "UtilityStructures.h"

#include <stringapiset.h>
#include "Animation.h"
struct M3dRawData
{
	void Clear()
	{
		vertices.clear();
		vertices.resize(0);
		indices.clear();
		indices.resize(0);
		subsets.clear();
		subsets.resize(0);
		mats.clear();
		mats.resize(0);
	}
	std::vector<vbPosNormalTexTangent> vertices;
	std::vector<UINT> indices;
	std::vector<Subset> subsets;
	std::vector<M3dMaterial> mats;
};

struct M3dRawSkinnedData
{
	void Clear()
	{
		vertices.clear();
		vertices.resize(0);
		indices.clear();
		indices.resize(0);
		subsets.clear();
		subsets.resize(0);
		mats.clear();
		mats.resize(0);
	}
	std::vector<vbSkinnedVertex> vertices;
	std::vector<UINT> indices;
	std::vector<Subset> subsets;
	std::vector<M3dMaterial> mats;
	SkinnedData skinnedInfo;

};

struct AssimpRawData
{
	std::vector<vbPosNormalTexTangent> vertices;
	std::vector<UINT> indices;
	std::vector<Subset> subsets;
	std::vector<M3dMaterial> mats;
	std::vector<DirectX::XMFLOAT4X4> worlds;
	DirectX::XMMATRIX scale;
};
class M3dLoader
{
public:
	M3dLoader(const std::string& filename, bool skinned, bool assimp);
	M3dRawData rawData;
	M3dRawSkinnedData rawSkinnedData;
	void LoadAssimp(const std::string& filename,
					AssimpRawData& rawData,
					const DirectX::XMMATRIX& scale,
					const std::wstring& diffuseMapName,
					const std::wstring normalMapName);

	bool LoadM3d(const std::string& filename,
		std::vector<vbPosNormalTexTangent>& vertices,
		std::vector<UINT>& indices,
		std::vector<Subset>& subsets,
		std::vector<M3dMaterial>& mats);
	bool LoadM3d(const std::string& filename,
		std::vector<vbSkinnedVertex>& vertices,
		std::vector<UINT>& indices,
		std::vector<Subset>& subsets,
		std::vector<M3dMaterial>& mats,
		SkinnedData& skinInfo);

	//DirectXTex requires names to be in wchar.
	std::wstring utf8toUtf16(const std::string& str);
	

	void ReadMaterials(std::ifstream& fin, UINT numMaterials, std::vector<M3dMaterial>& mats);
	void ReadSubsetTable(std::ifstream& fin, UINT numSubsets, std::vector<Subset>& subsets);
	void ReadVertices(std::ifstream& fin, UINT numVertices, std::vector<vbPosNormalTexTangent>& vertices);
	void ReadTriangles(std::ifstream& fin, UINT numTriangles, std::vector<UINT>& indices);
	void ReadSkinnedVertices(std::ifstream& fin, UINT numVertices, std::vector<vbSkinnedVertex>& vertices);
	void ReadBoneOffsets(std::ifstream& fin, UINT numBones, std::vector<DirectX::XMFLOAT4X4>& boneOffsets);
	void ReadBoneHierarchy(std::ifstream& fin, UINT numBones, std::vector<int>& boneIndexToParentIndex);
	void ReadAnimationClips(std::ifstream& fin, UINT numBones, UINT numAnimationClips,
		std::unordered_map<std::string, AnimationClip>& animations);
	void ReadBoneKeyframes(std::ifstream& fin, UINT numBones, BoneAnimation& boneAnimation);
	DirectX::XMFLOAT4X4 ConvertAiMatrixToD3DMatrix(aiMatrix4x4& m);

};