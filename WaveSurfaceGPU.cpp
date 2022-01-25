#include "WaveSurfaceGPU.h"

WaveSurfaceGPU::WaveSurfaceGPU(Graphics& gfx)
{
	geoGen.CreateGrid((float)numColumns, (float)numRows, 200, 200, mesh);
	wave.Initialize(numRows, numColumns, 0.8f, 0.03f, 3.85f, 0.4f);
	waveConstant.x = wave.waveConstant[0];
	waveConstant.y = wave.waveConstant[1];
	waveConstant.z = wave.waveConstant[2];

	std::vector<vbPosNormalTex> vertices;
	vertices.resize(mesh.vertices.size());
	for (UINT i = 0; i < mesh.vertices.size(); ++i)
	{
		vertices[i].pos = mesh.vertices[i].position;
		vertices[i].normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertices[i].tex = mesh.vertices[i].TexC;
	}
	pVertexBuffer = gfx.CreateVertexBuffer(vertices, false, false, L"GPU waves vertex buffer");

	pIndexBuffer = gfx.CreateIndexBuffer(mesh.indices, L"GPU waves index buffer");
	indexCount = mesh.indices.size();
	vsConsts = GetAndBuildConstantBufferData();
	wavesMat.diffuseAlbedo = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 0.7f);
	wavesMat.fresnelR0 = DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f);
	wavesMat.shininess = 0.7f;


	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = numColumns;
	texDesc.Height = numRows;
	texDesc.MipLevels = 1u;
	texDesc.ArraySize = 1u;
	texDesc.Format = DXGI_FORMAT_R32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	std::vector<float> zero(numRows* numColumns, 0.0f);
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &zero[0];
	initData.SysMemPitch = numColumns * sizeof(float);

	gfx.pgfx_pDevice->CreateTexture2D(&texDesc, &initData, &previousSolutionTex);
	gfx.pgfx_pDevice->CreateTexture2D(&texDesc, &initData, &currentSolutionTex);
	gfx.pgfx_pDevice->CreateTexture2D(&texDesc, &initData, &nextWaveSolutionTex);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	gfx.pgfx_pDevice->CreateShaderResourceView(previousSolutionTex, &srvDesc, &pPreviousSolutionSRV);
	gfx.pgfx_pDevice->CreateShaderResourceView(currentSolutionTex, &srvDesc, &pCurrentSolutionSRV);
	gfx.pgfx_pDevice->CreateShaderResourceView(nextWaveSolutionTex, &srvDesc, &pNextSolutionSRV);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = DXGI_FORMAT_R32_FLOAT;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = 0;
	gfx.pgfx_pDevice->CreateUnorderedAccessView(previousSolutionTex, &uavDesc, &pPreviousSolutionUAV);
	gfx.pgfx_pDevice->CreateUnorderedAccessView(currentSolutionTex, &uavDesc, &pCurrentSolutionUAV);
	gfx.pgfx_pDevice->CreateUnorderedAccessView(nextWaveSolutionTex, &uavDesc, &pNextSolutionUAV);

	previousSolutionTex->Release();
	currentSolutionTex->Release();
	nextWaveSolutionTex->Release();

}

ID3D11Buffer** WaveSurfaceGPU::GetVertexBuffer()
{
	return &pVertexBuffer;
}

ID3D11Buffer* WaveSurfaceGPU::GetIndexBuffer()
{
	return pIndexBuffer;
}

UINT WaveSurfaceGPU::GetIndexCount()
{
	return indexCount;
}

cbGPUWavesVSConstData WaveSurfaceGPU::GetAndBuildConstantBufferData()
{
	vsConsts.spatialStep = wave.SpatialStep();
	vsConsts.displacementMapTexelSize[0] = 1.0f / numColumns;
	vsConsts.displacementMapTexelSize[1] = 1.0f / numRows;
	return vsConsts;
}
