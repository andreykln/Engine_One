#pragma once
#include "Shape.h"

class M3dLoader
{
public:
	bool LoadM3d(const std::string& filename,
		std::vector<vbPosNormalTexTangent>& vertices,
		std::vector<UINT>& indices,
		std::vector<Subset>& subsets,
		std::vector<M3dMaterial>& mats);

private:
	//DirectXTex requires names to be in wchar.
	std::wstring utf8toUtf16(const std::string& str);


	void ReadMaterials(std::ifstream& fin, UINT numMaterials, std::vector<M3dMaterial>& mats);
	void ReadSubsetTable(std::ifstream& fin, UINT numSubsets, std::vector<Subset>& subsets);
	void ReadVertices(std::ifstream& fin, UINT numVertices, std::vector<vbPosNormalTexTangent>& vertices);
	void ReadTriangles(std::ifstream& fin, UINT numTriangles, std::vector<UINT>& indices);
};