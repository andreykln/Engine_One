#include "M3dLoader.h"

M3dLoader::M3dLoader(const std::string& filename, bool skinned, bool assimp)
{
	if (skinned)
	{
		bool loaded = LoadM3d(filename, rawSkinnedData.vertices, rawSkinnedData.indices, rawSkinnedData.subsets,
			rawSkinnedData.mats, rawSkinnedData.skinnedInfo);
	}
	else if(assimp)
	{
// 		LoadAssimp(filename, rawData.vertices, rawData.indices, rawData.subsets, rawData.mats);
	}
	else
	{
		bool loaded = LoadM3d(filename, rawData.vertices, rawData.indices, rawData.subsets, rawData.mats);
	}
}

void M3dLoader::LoadAssimp(const std::string& filename,
	AssimpRawData& rawData,
	const DirectX::XMMATRIX& scale,
	const std::wstring& diffuseMapName,
	const std::wstring normalMapName)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename,
		aiProcess_ConvertToLeftHanded |
		aiProcess_CalcTangentSpace |
		aiProcess_JoinIdenticalVertices |
		aiProcess_Triangulate |
		aiProcess_FixInfacingNormals |
		aiProcess_FindInvalidData | 
		aiProcess_GenSmoothNormals |
		aiProcess_GenUVCoords);

	M3dMaterial mat;
	mat.diffuseAlbedo = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mat.fresnelR0 = DirectX::XMFLOAT3(0.05f, 0.05f, 0.05f);
	mat.shininess = 0.2f;
	mat.alphaClip = false;
	mat.diffuseMapName = diffuseMapName;
	mat.normalMapName = normalMapName;

	importer.ApplyPostProcessing(aiProcess_CalcTangentSpace);
	std::string err = importer.GetErrorString();
	M3dRawData* pRawModel = new M3dRawData;
	UINT numMesh = 0;
	UINT fStart = 0;
	UINT baseVertLoc = 0;
	if (scene->HasMeshes())
	{
		//get transforms
		const UINT numOfChildren = scene->mRootNode->mNumChildren;
		aiMatrix4x4* childrenTransforms = new aiMatrix4x4[numOfChildren];
		rawData.worlds.resize(numOfChildren);
		if (numOfChildren == 0)
		{
			DirectX::XMFLOAT4X4 t; 
			DirectX::XMStoreFloat4x4(&t, DirectX::XMMatrixIdentity());
			rawData.worlds.push_back(t);
		}
		for (int h = 0; h < numOfChildren; h++)
		{
			
			rawData.worlds[h] = ConvertAiMatrixToD3DMatrix(scene->mRootNode->mChildren[h]->mTransformation);
		}
	

		numMesh = scene->mNumMeshes;
		for (UINT i = 0; i < numMesh; i++)
		{
			aiMesh* t  = scene->mMeshes[i];
			const UINT nVert = t->mNumVertices;

			for (UINT j = 0; j < nVert; j++)
			{
				vbPosNormalTexTangent v;
				aiVector3D tVec;
				v.pos.x = t->mVertices[j].x;
				v.pos.y = t->mVertices[j].y;
				v.pos.z = t->mVertices[j].z;
				v.normal.x = t->mNormals[j].x;
				v.normal.y = t->mNormals[j].y;
				v.normal.z = t->mNormals[j].z;
				if (t->mTangents)
				{
					v.tangent.x = t->mTangents[j].x;
					v.tangent.y = t->mTangents[j].y;
					v.tangent.z = t->mTangents[j].z;
				}
				if (t->mTextureCoords[0])
				{
					v.tex.x = t->mTextureCoords[0][j].x;
					v.tex.y = t->mTextureCoords[0][j].y;
				}
				pRawModel->vertices.push_back(v);
			}
			for (UINT k = 0; k < t->mNumFaces; k++)
			{
				const aiFace& face = t->mFaces[k];
				pRawModel->indices.push_back(face.mIndices[0]);
				pRawModel->indices.push_back(face.mIndices[1]);
				pRawModel->indices.push_back(face.mIndices[2]);

			}
			Subset sbs;
			sbs.ID = i;
			sbs.FaceCount = t->mNumFaces;
			sbs.FaceStart = fStart;
			sbs.VertexCount = nVert;
			sbs.VertexStart = baseVertLoc;
			baseVertLoc += sbs.VertexCount;
			fStart += sbs.FaceCount;
			pRawModel->subsets.push_back(sbs);
			pRawModel->mats.push_back(mat);

		}


		delete[] childrenTransforms;
	}

	if (rawData.worlds.size() < numMesh)
	{
		DirectX::XMMATRIX t = DirectX::XMMatrixIdentity();
		DirectX::XMFLOAT4X4 t0;
		DirectX::XMStoreFloat4x4(&t0, t);
		for (size_t i = rawData.worlds.size(); i < numMesh; i++)
		{
			rawData.worlds.push_back(t0);
		}
	}
	rawData.indices.resize(pRawModel->indices.size());
	rawData.indices = pRawModel->indices;
	rawData.mats.resize(pRawModel->mats.size());
	rawData.mats = pRawModel->mats;
	rawData.subsets.resize(pRawModel->subsets.size());
	rawData.subsets = pRawModel->subsets;
	rawData.vertices.resize(pRawModel->vertices.size());
	rawData.vertices = pRawModel->vertices;
	rawData.scale = scale;


	delete pRawModel;
	pRawModel = nullptr;
}

void M3dLoader::LoadSponza(const std::string& filename,
							const std::string& matFileName,
							AssimpRawData& rawData,
							const DirectX::XMMATRIX& scale)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename,
		aiProcess_ConvertToLeftHanded |
		aiProcess_CalcTangentSpace |
		aiProcess_JoinIdenticalVertices |
		aiProcess_Triangulate |
		aiProcess_FixInfacingNormals |
		aiProcess_FindInvalidData |
		aiProcess_GenSmoothNormals |
		aiProcess_GenUVCoords);


	importer.ApplyPostProcessing(aiProcess_CalcTangentSpace);
	std::string err = importer.GetErrorString();
	M3dRawData* pRawModel = new M3dRawData;
	UINT numMesh = 0;
	UINT fStart = 0;
	UINT baseVertLoc = 0;
	if (scene->HasMeshes())
	{
		//get transforms
		const UINT numOfChildren = scene->mRootNode->mNumChildren;
		aiMatrix4x4* childrenTransforms = new aiMatrix4x4[numOfChildren];
		rawData.worlds.resize(numOfChildren);
		if (numOfChildren == 0)
		{
			DirectX::XMFLOAT4X4 t;
			DirectX::XMStoreFloat4x4(&t, DirectX::XMMatrixIdentity());
			rawData.worlds.push_back(t);
		}
		for (UINT h = 0; h < numOfChildren; h++)
		{

			rawData.worlds[h] = ConvertAiMatrixToD3DMatrix(scene->mRootNode->mChildren[h]->mTransformation);
		}
		std::vector<std::string> matNames;

// 		for (int i = 0; i < scene->mNumMaterials; i++)
// 		{
// 			matNames.emplace_back(scene->mMaterials[i]->GetName().C_Str());
// 		}
		//edit some names manually so they have sensible names
		//TODO delete?
// 		matNames[0] = "fabric_red";
// 		matNames[2] = "vase_plant";
// 		matNames[4] = "background";
// 		matNames[5] = "fabric_red";
// 		matNames[14] = "UNKNOWN0";
// 		matNames[25] = "lion";
// 		matNames[16] = "fabric_green";
// 		matNames[17] = "fabric_blue";
// 		matNames[18] = "fabric_red";
// 		matNames[19] = "curtain_blue";
// 		matNames[20] = "curtain_red";
// 		matNames[21] = "curtain_green";


		std::ifstream fin(matFileName);
		for (UINT i = 0; i < scene->mNumMaterials; i++)
		{
			M3dMaterial mat;
			mat = ReadSponzaMat(matFileName, fin);
			pRawModel->mats.push_back(mat);
		}

		numMesh = scene->mNumMeshes;
		//remove outer walls
		numMesh -= 4;
		for (UINT i = 0; i < numMesh; i++)
		{
			aiMesh* t = scene->mMeshes[i];
			const UINT nVert = t->mNumVertices;
			float fix = 1.0f;
			//reverse normals for the ceiling
			if (i == 8 || i == 19)
			{
				fix = -1.0f;
			}
			else
			{
				fix = 1.0f;
			}
			for (UINT j = 0; j < nVert; j++)
			{
				vbPosNormalTexTangent v;
				aiVector3D tVec;
				v.pos.x = t->mVertices[j].x;
				v.pos.y = t->mVertices[j].y;
				v.pos.z = t->mVertices[j].z;
				v.normal.x = t->mNormals[j].x * fix;
				v.normal.y = t->mNormals[j].y * fix;
				v.normal.z = t->mNormals[j].z * fix;
				if (t->mTangents)
				{
					v.tangent.x = t->mTangents[j].x;
					v.tangent.y = t->mTangents[j].y;
					v.tangent.z = t->mTangents[j].z;
				}
				if (t->mTextureCoords[0])
				{
					v.tex.x = t->mTextureCoords[0][j].x;
					v.tex.y = t->mTextureCoords[0][j].y;
				}
				pRawModel->vertices.push_back(v);
			}
			for (UINT k = 0; k < t->mNumFaces; k++)
			{
				const aiFace& face = t->mFaces[k];
				pRawModel->indices.push_back(face.mIndices[0]);
				pRawModel->indices.push_back(face.mIndices[1]);
				pRawModel->indices.push_back(face.mIndices[2]);

			}
			Subset sbs;
			sbs.ID = scene->mMeshes[i]->mMaterialIndex;
			sbs.FaceCount = t->mNumFaces;
			sbs.FaceStart = fStart;
			sbs.VertexCount = nVert;
			sbs.VertexStart = baseVertLoc;
			baseVertLoc += sbs.VertexCount;
			fStart += sbs.FaceCount;
			pRawModel->subsets.push_back(sbs);
		}


		delete[] childrenTransforms;
	}

	if (rawData.worlds.size() < numMesh)
	{
		DirectX::XMMATRIX t = DirectX::XMMatrixIdentity();
		DirectX::XMFLOAT4X4 t0;
		DirectX::XMStoreFloat4x4(&t0, t);
		for (size_t i = rawData.worlds.size(); i < numMesh; i++)
		{
			rawData.worlds.push_back(t0);
		}
	}
	rawData.indices.resize(pRawModel->indices.size());
	rawData.indices = pRawModel->indices;
	rawData.mats.resize(pRawModel->mats.size());
	rawData.mats = pRawModel->mats;
	rawData.subsets.resize(pRawModel->subsets.size());
	rawData.subsets = pRawModel->subsets;
	rawData.vertices.resize(pRawModel->vertices.size());
	rawData.vertices = pRawModel->vertices;
	rawData.scale = scale;


	delete pRawModel;
	pRawModel = nullptr;
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

M3dMaterial M3dLoader::ReadSponzaMat(const std::string& filename, std::ifstream& fstream)
{
	M3dMaterial m;
	std::string ignore;
	std::string tempname;
	fstream >> ignore;
	fstream >> m.name;
	fstream >> ignore >> m.diffuseAlbedo.x >> m.diffuseAlbedo.y >> m.diffuseAlbedo.z >> m.diffuseAlbedo.w;
	fstream >> ignore >> m.fresnelR0.x >> m.fresnelR0.y >> m.fresnelR0.z;
	fstream >> ignore >> m.shininess;
	fstream >> ignore >> tempname;
	m.diffuseMapName = utf8toUtf16(tempname);
	tempname.clear();
	fstream >> ignore >> tempname;
	m.normalMapName = utf8toUtf16(tempname);
	return m;
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

DirectX::XMFLOAT4X4 M3dLoader::ConvertAiMatrixToD3DMatrix(aiMatrix4x4& m)
{
	DirectX::XMFLOAT4X4 M = {
		m.a1, m.b1, m.c1, m.d1,
		m.a2, m.b2, m.c2, m.d2,
		m.a3, m.b3, m.c3, m.d3,
		m.a4, m.b4, m.c4, m.d4 };
	return M;
}
