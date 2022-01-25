#include "Terrain.h"

Terrain::Terrain(Graphics& gfx)
{
	terrainInitInfo.HeightMapFilename = L"Textures\\Terrain\\terrain.raw";
	terrainInitInfo.cellSpacing = 0.5f; 
	terrainInitInfo.HeightMapHeight = 2049;
	terrainInitInfo.HeightMapWidth = 2049;
	terrainInitInfo.HeightScale = 50.0f;

	terrainConstants.texelCellSpaceU = 1.0f / terrainInitInfo.HeightMapWidth;
	terrainConstants.texelCellSpaceV = 1.0f / terrainInitInfo.HeightMapHeight;
	terrainConstants.worldCellSpace = terrainInitInfo.cellSpacing;


	numPatchVertRows = ((terrainInitInfo.HeightMapHeight - 1) / cellsPerPatch) + 1;
	numPatchVertCols = ((terrainInitInfo.HeightMapWidth - 1) / cellsPerPatch) + 1;
	numPatchQuadFaces = (numPatchVertRows - 1) * (numPatchVertCols - 1);
	std::vector<unsigned char> in(terrainInitInfo.HeightMapWidth * terrainInitInfo.HeightMapHeight);

	std::ifstream inFile;
	inFile.open(terrainInitInfo.HeightMapFilename.c_str(), std::ios_base::binary);
	inFile.read((char*)&in[0], (std::streamsize)in.size());
	inFile.close();

	heightMap.resize(terrainInitInfo.HeightMapWidth * terrainInitInfo.HeightMapHeight, 0.0f);
	for (UINT i = 0; i < terrainInitInfo.HeightMapWidth * terrainInitInfo.HeightMapHeight; ++i)
	{
		heightMap[i] = (in[i] / 255.0f) * terrainInitInfo.HeightScale;
	}

	Smooth();


	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Format = DXGI_FORMAT_R16_FLOAT;
	texDesc.ArraySize = 1u;
	texDesc.MipLevels = 1u;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0u;
	texDesc.MiscFlags = 0u;
	texDesc.SampleDesc.Count = 1u;
	texDesc.SampleDesc.Quality = 0u;
	texDesc.Height = terrainInitInfo.HeightMapHeight;
	texDesc.Width = terrainInitInfo.HeightMapWidth;

	std::vector<DirectX::PackedVector::HALF> hMap(heightMap.size());
	std::transform(heightMap.begin(), heightMap.end(), hMap.begin(), DirectX::PackedVector::XMConvertFloatToHalf);

	D3D11_SUBRESOURCE_DATA subresData;
	subresData.pSysMem = hMap.data();
	subresData.SysMemPitch = terrainInitInfo.HeightMapWidth * sizeof(DirectX::PackedVector::HALF);
	subresData.SysMemSlicePitch = 0u;

	ID3D11Texture2D* hMapTex = nullptr;
	gfx.pgfx_pDevice->CreateTexture2D(&texDesc, &subresData, &hMapTex);
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = -1;
	srvDesc.Texture2D.MostDetailedMip = 0u;
	gfx.pgfx_pDevice->CreateShaderResourceView(hMapTex, &srvDesc, &pHeightMapVS);
	gfx.pgfx_pDevice->CreateShaderResourceView(hMapTex, &srvDesc, &pHeightMapDS);
	gfx.pgfx_pDevice->CreateShaderResourceView(hMapTex, &srvDesc, &pHeightMapPS);

	hMapTex->Release();

	//Vertex buffer
	std::vector<vbPosTexBoundsY> patchVertices((uint64_t)numPatchVertCols * (uint64_t)numPatchVertRows);
	float halfWidth = 0.5f * GetWidth();
	float halfDepth = 0.5f * GetDepth();

	float patchWidth = GetWidth() / (numPatchVertCols - 1);
	float patchDepth = GetDepth() / (numPatchVertRows - 1);
	float du = 1.0f / (numPatchVertCols - 1);
	float dv = 1.0f / (numPatchVertRows - 1);

	for (UINT i = 0; i < numPatchVertRows; ++i)
	{
		float z = halfDepth - i * patchDepth;
		for (UINT j = 0; j < numPatchVertCols; ++j)
		{
			float x = -halfWidth + j * patchWidth;
			patchVertices[(uint64_t)i * numPatchVertCols + j].pos = DirectX::XMFLOAT3(x, 0.0f, z);
			patchVertices[(uint64_t)i * numPatchVertCols + j].tex.x = j * du;
			patchVertices[(uint64_t)i * numPatchVertCols + j].tex.y = i * dv;
		}
	}

	CalculateAllPatchBoundsY();
	//store axis-aligned bounding box y-bounds in upper left patch-corner

	for (UINT i = 0; i < numPatchVertRows - 1; ++i)
	{
		for (UINT j = 0; j < numPatchVertCols - 1; ++j)
		{
			UINT patchID = i * (numPatchVertCols - 1) + j;
			patchVertices[(uint64_t)i * numPatchVertCols + j].boundsY = patchBoundsY[patchID];
		}
	}

	pVertexBuffer = gfx.CreateVertexBuffer(patchVertices, false, false, L"TerrainGrid");
	//TODO where is this used
// 	numPatchVertices = numPatchVertRows * numPatchVertCols;

	//Index buffer
	numPatchQuadFaces = (numPatchVertCols - 1) * (numPatchVertRows - 1);
	std::vector<UINT> indices((uint64_t)numPatchQuadFaces * 4u); //4 indices per quad face

	UINT k = 0;
	for (UINT i = 0; i < numPatchVertRows - 1; ++i)
	{
		for (UINT j = 0; j < numPatchVertCols - 1; ++j)
		{
			//top row of 2x2 quad patch
			indices[k] = i * numPatchVertCols + j;
			indices[(uint64_t)k+1u] = i * numPatchVertCols + j + 1;

			//bottom row of 2x2 quad patch
			indices[(uint64_t)k + 2u] = (i+1u) * numPatchVertCols + j;
			indices[(uint64_t)k + 3u] = (i + 1u) * numPatchVertCols + j + 1;
			k += 4; //next quad
		}
	}

	pIndexBuffer = gfx.CreateIndexBuffer(indices, L"Terrain indices");
	indexCount = static_cast<UINT>(indices.size());

	terrainMaterial.diffuseAlbedo = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	terrainMaterial.fresnelR0 = DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f);
	terrainMaterial.shininess = 0.05f;
	

}

ID3D11Buffer** Terrain::GetVertexBuffer()
{
	return &pVertexBuffer;
}

ID3D11Buffer* Terrain::GetIndexBuffer()
{
	return pIndexBuffer;
}

UINT Terrain::GetIndexCount()
{
	return indexCount;
}

int Terrain::GetNumQuadFaces()
{
	return numPatchQuadFaces;
}

float Terrain::GetHeight(float camX, float camZ, float camY)
{
	//transform from terrain local space to "cell" space
	float c = (camX + 0.5f * GetWidth()) /  terrainInitInfo.cellSpacing;
	float d = (camZ - 0.5f * GetWidth()) / -terrainInitInfo.cellSpacing;

	//get the row and column we are in
	int row = (int)std::floorf(d);
	int col = (int)std::floorf(c);

	// Grab the heights of the cell we are in.
	// A*--*B
	//  | /|
	//  |/ |
	// C*--*D
	float A = heightMap[row * terrainInitInfo.HeightMapWidth + col];
	float B = heightMap[row * terrainInitInfo.HeightMapWidth + col + 1];
	float C = heightMap[(row + 1) * terrainInitInfo.HeightMapWidth + col];
	float D = heightMap[(row + 1) * terrainInitInfo.HeightMapWidth + col + 1];

	float s = c - (float)col;
	float t = d - (float)row;
	//if upper triangle ABC
	if (s + t <= 1.0f)
	{
		float uy = B - C;
		float vy = C - A;
		return A + s * uy + t * vy;
	} 
	else
	{
		float uy = C - D;
		float vy = B - D;
		return D + (1.0f - s) * uy + (1.0f - t) * vy;
	}
}

bool Terrain::InBounds(int i, int j)
{
	bool inBounds = 
		i >= 0 && i < (int)terrainInitInfo.HeightMapHeight &&
		j >= 0 && j < (int)terrainInitInfo.HeightMapWidth;
	return inBounds;
}

float Terrain::Average(int i, int j)
{
	float total = 0.0f;
	float number = 0.0f;
	for (int m =  i - 1; m <= i + 1; ++m)
	{
		for (int n = j - 1; n <= j + 1; ++n)
		{
			if (InBounds(m, n))
			{
				total += heightMap[(uint64_t)m * terrainInitInfo.HeightMapWidth + n];
				number += 1.0f;
			}
		}
	}
	return total / number;
}

void Terrain::Smooth()
{
	std::vector<float> destination(heightMap.size());
	for (UINT i = 0; i < terrainInitInfo.HeightMapHeight; ++i)
	{
		for (UINT j = 0; j < terrainInitInfo.HeightMapWidth; ++j)
		{
			destination[(uint64_t)i * terrainInitInfo.HeightMapWidth + j] = Average(i, j);
		}
	}
	heightMap = destination;
}

void Terrain::CalculateAllPatchBoundsY()
{
	patchBoundsY.resize(numPatchQuadFaces);

	// for each patch
	for (UINT i = 0; i < numPatchVertRows - 1; ++i)
	{
		for (UINT j = 0; j < numPatchVertCols - 1; ++j)
		{
			CalculatePatchBoundsY(i, j);
		}
	}
}

void Terrain::CalculatePatchBoundsY(UINT i, UINT j)
{
	//scan the heightmap values this patch covers and
	//compute the min/max height
	UINT x0 = j * cellsPerPatch;
	UINT x1 = (j + 1) * cellsPerPatch;

	UINT y0 = i * cellsPerPatch;
	UINT y1 = (i + 1) * cellsPerPatch;

	float minY = FLT_MAX;
	float maxY = -FLT_MAX;

	for (UINT y = y0; y <= y1; ++y)
	{
		for (UINT x = x0; x <= x1; ++x)
		{
			UINT k = y * terrainInitInfo.HeightMapWidth + x;
			minY = MathHelper::Min(minY, heightMap[k]);
			maxY = MathHelper::Max(maxY, heightMap[k]);
		}
	}
	UINT patchID = i * (numPatchVertCols - 1) + j;
	patchBoundsY[patchID] = DirectX::XMFLOAT2(minY, maxY);
}

float Terrain::GetWidth()
{
	//total width
	return (terrainInitInfo.HeightMapWidth - 1) * terrainInitInfo.cellSpacing;
}

float Terrain::GetDepth()
{
	//total depth
	return (terrainInitInfo.HeightMapHeight - 1) * terrainInitInfo.cellSpacing;
}

