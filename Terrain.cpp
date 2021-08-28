#include "Terrain.h"

Terrain::Terrain(Graphics& gfx)
{
	terrainInitInfo.BlendMapFilename = L"Textures\\Terrain\\blend.dds";
	terrainInitInfo.LayerMapFilename0 = L"Textures\\Terrain\\grass.dds";
	terrainInitInfo.LayerMapFilename1 = L"Textures\\Terrain\\darkdirt.dds";
	terrainInitInfo.LayerMapFilename2 = L"Textures\\Terrain\\stone.dds";
	terrainInitInfo.LayerMapFilename3 = L"Textures\\Terrain\\lightdirt.dds";
	terrainInitInfo.LayerMapFilename4 = L"Textures\\Terrain\\snow.dds";
	terrainInitInfo.HeightMapFilename = L"Textures\\Terrain\\terrain.raw";
	terrainInitInfo.cellSpacing = 0.5f;
	terrainInitInfo.HeightMapHeight = 2049;
	terrainInitInfo.HeightMapWidth = 2049;
	terrainInitInfo.HeightScale = 50.0f;

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
	gfx.pgfx_pDevice->CreateShaderResourceView(hMapTex, &srvDesc, &pHeightMapSRV);

	hMapTex->Release();

	//Vertex buffer
	std::vector<TerrainVB> patchVertices((int)numPatchVertCols * (int)numPatchVertRows);
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
			patchVertices[i * numPatchVertCols + j].pos = DirectX::XMFLOAT3(x, 0.0f, z);
			patchVertices[i * numPatchVertCols + j].tex.x = j * du;
			patchVertices[i * numPatchVertCols + j].tex.y = j * dv;
		}
	}
	CalculateAllPatchBoundsY();
	//store axis-aligned bounding box y-bounds in upper left patch-corner

	auto a = std::size(patchBoundsY);
	auto b = std::size(patchVertices);
	for (UINT i = 0; i < numPatchVertRows - 1; ++i)
	{
		for (UINT j = 0; j < numPatchVertCols - 1; ++j)
		{
			UINT patchID = i * (numPatchVertCols - 1) + j;
			patchVertices[(int)i * (int)numPatchVertCols + (int)j].boundsY = patchBoundsY[patchID];
		}
	}

	VertexBuffer* pVB = new VertexBuffer(gfx, patchVertices, L"TerrainGrid_");
	AddBind(pVB);

	//TODO where is this used
	numPatchVertices = numPatchVertRows * numPatchVertCols;

	//Index buffer
	numPatchQuadFaces = (numPatchVertCols - 1) * (numPatchVertRows - 1);
	std::vector<UINT> indices(numPatchQuadFaces * 4u); //4 indices per quad face

	int k = 0;
	for (UINT i = 0; i < numPatchVertRows - 1; ++i)
	{
		for (UINT j = 0; j < numPatchVertCols - 1; ++j)
		{
			//top row of 2x2 quad patch
			indices[k] = i * numPatchVertCols + j;
			indices[k+1u] = i * numPatchVertCols + j + 1;

			//bottom row of 2x2 quad patch
			indices[k + 2u] = (i+1u) * numPatchVertCols + j;
			indices[k + 3u] = (i + 1u) * numPatchVertCols + j + 1;
			k += 4; //next quad
		}
	}

	

	IndexBuffer* pIB = new IndexBuffer(gfx, indices, L"TerrainIndices_");
	AddIndexBuffer(pIB);
	TextureSampler* pTexSamplerVS = new TextureSampler(gfx, ShaderType::Vertex);
	AddBind(pTexSamplerVS);


	HullShaderConstantBuffer<CB_HS_TerrainPerFrame>* pHSCB = 
		new HullShaderConstantBuffer(gfx, HullShaderCB, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pHSBufferCopy = pHSCB->GetHullShaderConstantBuffer();
	AddBind(pHSCB);

	DomainShaderConstantBuffer<CB_VS_WorldViewProjection>* pDSCB =
		new DomainShaderConstantBuffer(gfx, DomainShaderCB, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pDSBufferCopy = pDSCB->GetDomainShaderConstantBuffer();
	AddBind(pDSCB);

	std::wstring layers[5];
	layers[0] = terrainInitInfo.LayerMapFilename0;
	layers[1] = terrainInitInfo.LayerMapFilename1;
	layers[2] = terrainInitInfo.LayerMapFilename2;
	layers[3] = terrainInitInfo.LayerMapFilename3;
	layers[4] = terrainInitInfo.LayerMapFilename4;



	ShaderResourceView* pLayerMaps = new ShaderResourceView(layers, std::size(layers));
	pTerrainLayerMaps = pLayerMaps->GetTextureArray(gfx);


}

void Terrain::SetSRVAndCBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPosition, DirectX::XMFLOAT4 worldFrustumPlanes[6], 
	DirectX::XMMATRIX WVP)
{
	gfx.pgfx_pDeviceContext->VSSetShaderResources(0u, 1u, &pHeightMapSRV);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pHSBufferCopy, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CB_HS_TerrainPerFrame* pHullShader = reinterpret_cast<CB_HS_TerrainPerFrame*>(mappedData.pData);
	pHullShader->cameraPosition = camPosition;
	pHullShader->worldFrustumPlanes[0] = worldFrustumPlanes[0];
	pHullShader->worldFrustumPlanes[1] = worldFrustumPlanes[1];
	pHullShader->worldFrustumPlanes[2] = worldFrustumPlanes[2];
	pHullShader->worldFrustumPlanes[3] = worldFrustumPlanes[3];
	pHullShader->worldFrustumPlanes[4] = worldFrustumPlanes[4];
	pHullShader->worldFrustumPlanes[5] = worldFrustumPlanes[5];

	gfx.pgfx_pDeviceContext->Unmap(pHSBufferCopy, 0u);

	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pDSBufferCopy, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CB_VS_WorldViewProjection* pDomainShader = reinterpret_cast<CB_VS_WorldViewProjection*>(mappedData.pData);
	pDomainShader->worldViewProjection = WVP;
	gfx.pgfx_pDeviceContext->Unmap(pDSBufferCopy, 0u);



}

bool Terrain::InBounds(int i, int j)
{
	bool inBounds = i >= 0 && i < terrainInitInfo.HeightMapHeight && j >= 0 && terrainInitInfo.HeightMapWidth;
	return inBounds;
}

float Terrain::Average(int i, int j)
{
	float total = 0.0f;
	float number = 0.0f;
	for (int m =  i - 1; m < i + 1; ++m)
	{
		for (int n = j - 1; n < j + 1; ++n)
		{
			if (InBounds(m, n))
			{
				total += heightMap[m * terrainInitInfo.HeightMapWidth + n];
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
			destination[i * terrainInitInfo.HeightMapWidth + j] = Average(i, j);
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

