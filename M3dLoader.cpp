#include "M3dLoader.h"

M3dLoader::M3dLoader(const std::string& filename)
{
	bool loaded = LoadM3d(filename, rawData.vertices, rawData.indices, rawData.subsets, rawData.mats);

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
