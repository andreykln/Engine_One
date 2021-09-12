#include "Terrain.h"

Terrain::Terrain(Graphics& gfx)
{
	cbDirectionalLight.dirLight[0].ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	cbDirectionalLight.dirLight[0].diffuse = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	cbDirectionalLight.dirLight[0].direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	cbDirectionalLight.dirLight[0].specular = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

	cbDirectionalLight.dirLight[1].ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	cbDirectionalLight.dirLight[1].diffuse = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
	cbDirectionalLight.dirLight[1].direction = DirectX::XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);
	cbDirectionalLight.dirLight[1].specular = DirectX::XMFLOAT4(0.35f, 0.35f, 0.35f, 1.0f);

	cbDirectionalLight.dirLight[2].ambient = DirectX::XMFLOAT4(0.5, 0.5f, 0.5f, 1.0f);
	cbDirectionalLight.dirLight[2].diffuse = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
	cbDirectionalLight.dirLight[2].direction = DirectX::XMFLOAT3(0.0f, -0.707f, -0.707f);
	cbDirectionalLight.dirLight[2].specular = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

	cbDirectionalLight.mat.ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	cbDirectionalLight.mat.diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	cbDirectionalLight.mat.specular = DirectX::XMFLOAT4(0.15f, 0.15f, 0.15f, 8.0f);
	cbDirectionalLight.mat.reflect = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	PixelShaderConstantBuffer<CB_PS_DirectionalL_Fog>* pCBDirLight =
		new PixelShaderConstantBuffer(gfx, cbDirectionalLight, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	AddBind(pCBDirLight);

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

	terrainConstants.texelCellSpaceU = 1.0f / terrainInitInfo.HeightMapWidth;
	terrainConstants.texelCellSpaceV = 1.0f / terrainInitInfo.HeightMapHeight;
	terrainConstants.worldCellSpace = terrainInitInfo.cellSpacing;
	PixelShaderConstantBuffer<CB_PS_Terrain>* pCBPSTerrainContants =
		new PixelShaderConstantBuffer(gfx, terrainConstants, 2u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	AddBind(pCBPSTerrainContants);
	CB_PS_PerFrameUpdate;

	PixelShaderConstantBuffer<CB_PS_PerFrameUpdate>* pCBPSPerFrame =
		new PixelShaderConstantBuffer(gfx, cbPsPerFrame, 1u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pPSBufferCopy = pCBPSPerFrame->GetPixelShaderConstantBuffer();
	AddBind(pCBPSPerFrame);


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
	std::vector<TerrainVB> patchVertices((uint64_t)numPatchVertCols * (uint64_t)numPatchVertRows);
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

	VertexBuffer* pVB = new VertexBuffer(gfx, patchVertices, L"TerrainGrid_");
	AddBind(pVB);

	//TODO where is this used
	numPatchVertices = numPatchVertRows * numPatchVertCols;

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

	IndexBuffer* pIB = new IndexBuffer(gfx, indices, L"TerrainIndices_");
	AddIndexBuffer(pIB);
	TextureSampler* pTexSamplerVS = new TextureSampler(gfx, ShaderType::Vertex);
	AddBind(pTexSamplerVS);

	TextureSampler* pTexSamplerPS = new TextureSampler(gfx, ShaderType::Pixel);
	AddBind(pTexSamplerPS);

	TextureSampler* pTexSamplerDS = new TextureSampler(gfx, ShaderType::Domain);
	AddBind(pTexSamplerDS);

	HullShaderConstantBuffer<CB_HS_TerrainPerFrame>* pHSCB = 
		new HullShaderConstantBuffer(gfx, HullShaderCB, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pHSBufferCopy = pHSCB->GetHullShaderConstantBuffer();
	AddBind(pHSCB);

	DomainShaderConstantBuffer<CB_VS_WorldViewProjection>* pDSCB =
		new DomainShaderConstantBuffer(gfx, DomainShaderCB, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pDSBufferCopy = pDSCB->GetDomainShaderConstantBuffer();
	AddBind(pDSCB);

	std::wstring layers[4];
	std::wstring snowLayer[1];
	layers[0] = terrainInitInfo.LayerMapFilename0;
	layers[1] = terrainInitInfo.LayerMapFilename1;
	layers[2] = terrainInitInfo.LayerMapFilename2;
	layers[3] = terrainInitInfo.LayerMapFilename3;
	snowLayer[0] = terrainInitInfo.LayerMapFilename4;

	ShaderResourceView* pLayerMaps = new ShaderResourceView(layers, std::size(layers));
	pTerrainLayerMaps = pLayerMaps->GetTextureArray(gfx);

	ShaderResourceView* pSnowLayer = new ShaderResourceView(gfx, snowLayer, 3u, 1u, ShaderType::Pixel);
	AddBind(pSnowLayer);

	std::wstring blendMap[1];
	blendMap[0]= terrainInitInfo.BlendMapFilename;
	ShaderResourceView* pBlendMap = new ShaderResourceView(gfx, blendMap, 2u, 1u, ShaderType::Pixel);
	AddBind(pBlendMap);

}

void Terrain::SetSRVAndCBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPosition, DirectX::XMMATRIX WVP)
{
	DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(-10.0f, -20.0f, 0.0f);
	DirectX::XMFLOAT4 worldPlanes[6];
	ExtractFrustumPlanes(worldPlanes, world * WVP);
	gfx.pgfx_pDeviceContext->VSSetShaderResources(0u, 1u, &pHeightMapVS);
	gfx.pgfx_pDeviceContext->DSSetShaderResources(0u, 1u, &pHeightMapDS);
	gfx.pgfx_pDeviceContext->PSSetShaderResources(1u, 1u, &pHeightMapPS);
	gfx.pgfx_pDeviceContext->PSSetShaderResources(0u, 1u, &pTerrainLayerMaps);


	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pHSBufferCopy, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CB_HS_TerrainPerFrame* pHullShader = reinterpret_cast<CB_HS_TerrainPerFrame*>(mappedData.pData);
	pHullShader->cameraPosition = camPosition;
	pHullShader->worldFrustumPlanes[0] = worldPlanes[0];
	pHullShader->worldFrustumPlanes[1] = worldPlanes[1];
	pHullShader->worldFrustumPlanes[2] = worldPlanes[2];
	pHullShader->worldFrustumPlanes[3] = worldPlanes[3];
	pHullShader->worldFrustumPlanes[4] = worldPlanes[4];
	pHullShader->worldFrustumPlanes[5] = worldPlanes[5];

	gfx.pgfx_pDeviceContext->Unmap(pHSBufferCopy, 0u);

	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pDSBufferCopy, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CB_VS_WorldViewProjection* pDomainShader = reinterpret_cast<CB_VS_WorldViewProjection*>(mappedData.pData);
	pDomainShader->worldViewProjection = DirectX::XMMatrixTranspose(WVP);
	pDomainShader->world = DirectX::XMMatrixTranspose(world);

	gfx.pgfx_pDeviceContext->Unmap(pDSBufferCopy, 0u);

	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pPSBufferCopy, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CB_PS_PerFrameUpdate* frame = reinterpret_cast<CB_PS_PerFrameUpdate*> (mappedData.pData);
	frame->cameraPositon = camPosition;

	if (GetAsyncKeyState('0') & 0x8000)
		frame->numberOfLights = 0;
	if (GetAsyncKeyState('1') & 0x8000)
		frame->numberOfLights = 1;
	if (GetAsyncKeyState('2') & 0x8000)
		frame->numberOfLights = 2;
	if (GetAsyncKeyState('3') & 0x8000)
		frame->numberOfLights = 3;

	gfx.pgfx_pDeviceContext->Unmap(pPSBufferCopy, 0u);

}

int Terrain::GetNumQuadFaces()
{
	return numPatchQuadFaces;
}

float Terrain::GetHeight(float camX, float camZ)
{
	//transform from terrain local space to "cell" space
	float c = (camX + 0.5f * GetWidth()) / terrainInitInfo.cellSpacing;
	float d = (camZ - 0.5f * GetWidth()) / -terrainInitInfo.cellSpacing;

	//get the row and column we are in
	int row = (float)std::floorf(d);
	int col = (float)std::floorf(c);

	// Grab the heights of the cell we are in.
	// A*--*B
	//  | /|
	//  |/ |
	// C*--*D
	float A = heightMap[(uint64_t)row * terrainInitInfo.HeightMapWidth + col];
	float B = heightMap[(uint64_t)row * terrainInitInfo.HeightMapWidth + col + 1];
	float C = heightMap[(uint64_t)(row + 1) * terrainInitInfo.HeightMapWidth + col];
	float D = heightMap[(uint64_t)(row + 1) * terrainInitInfo.HeightMapWidth + col + 1];

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
		return D + (1.0f - s) * uy + (1.0 - t) * vy;
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

void Terrain::ExtractFrustumPlanes(DirectX::XMFLOAT4 planes[6], DirectX::CXMMATRIX _M)
{
	using namespace DirectX;
	//
	// Left
	//
	DirectX::XMFLOAT4X4 M;
	DirectX::XMStoreFloat4x4(&M, _M);

	planes[0].x = M(0, 3) + M(0, 0);
	planes[0].y = M(1, 3) + M(1, 0);
	planes[0].z = M(2, 3) + M(2, 0);
	planes[0].w = M(3, 3) + M(3, 0);

	//
	// Right
	//
	planes[1].x = M(0, 3) - M(0, 0);
	planes[1].y = M(1, 3) - M(1, 0);
	planes[1].z = M(2, 3) - M(2, 0);
	planes[1].w = M(3, 3) - M(3, 0);

	//
	// Bottom
	//
	planes[2].x = M(0, 3) + M(0, 1);
	planes[2].y = M(1, 3) + M(1, 1);
	planes[2].z = M(2, 3) + M(2, 1);
	planes[2].w = M(3, 3) + M(3, 1);

	//
	// Top
	//
	planes[3].x = M(0, 3) - M(0, 1);
	planes[3].y = M(1, 3) - M(1, 1);
	planes[3].z = M(2, 3) - M(2, 1);
	planes[3].w = M(3, 3) - M(3, 1);

	//
	// Near
	//
	planes[4].x = M(0, 2);
	planes[4].y = M(1, 2);
	planes[4].z = M(2, 2);
	planes[4].w = M(3, 2);

	//
	// Far
	//
	planes[5].x = M(0, 3) - M(0, 2);
	planes[5].y = M(1, 3) - M(1, 2);
	planes[5].z = M(2, 3) - M(2, 2);
	planes[5].w = M(3, 3) - M(3, 2);

	// Normalize the plane equations.
	for (int i = 0; i < 6; ++i)
	{
		DirectX::XMVECTOR v = DirectX::XMPlaneNormalize(DirectX::XMLoadFloat4(&planes[i]));
		DirectX::XMStoreFloat4(&planes[i], v);
	}
}