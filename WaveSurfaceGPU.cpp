#include "WaveSurfaceGPU.h"

WaveSurfaceGPU::WaveSurfaceGPU(Graphics& gfx)
{
	geoGen.CreateGrid(numColumns, numRows, 60, 60, mesh);
	wave.Initialize(numRows, numColumns, 0.8f, 0.03f, 3.25f, 0.4f);

	directionalLight.mat.ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	directionalLight.mat.diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f);
	directionalLight.mat.specular = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 32.0f);

	directionalLight.dirLight[0].ambient = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 0.5f);
	directionalLight.dirLight[0].diffuse = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	directionalLight.dirLight[0].specular = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 0.5f);
	directionalLight.dirLight[0].direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	directionalLight.dirLight[1].ambient = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f);
	directionalLight.dirLight[1].diffuse = DirectX::XMFLOAT4(0.20f, 0.20f, 0.20f, 0.5f);
	directionalLight.dirLight[1].specular = DirectX::XMFLOAT4(0.65f, 0.65f, 0.65f, 0.5f);
	directionalLight.dirLight[1].direction = DirectX::XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

	directionalLight.dirLight[2].ambient = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f);
	directionalLight.dirLight[2].diffuse = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 0.5f);
	directionalLight.dirLight[2].specular = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 0.5f);
	directionalLight.dirLight[2].direction = DirectX::XMFLOAT3(0.0f, -0.707f, -0.707f);

	directionalLight.mat.ambient = DirectX::XMFLOAT4(0.0f, 0.427f, 0.557f, 0.5f);
	directionalLight.mat.diffuse = DirectX::XMFLOAT4(0.0f, 0.427f, 0.557f, 0.5f);
	directionalLight.mat.specular = DirectX::XMFLOAT4(0.55f, 0.55f, 0.55f, 64.0f);
	directionalLight.mat.reflect = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	vertices.resize(mesh.vertices.size());
	for (UINT i = 0; i < mesh.vertices.size(); ++i)
	{
		vertices[i].pos = mesh.vertices[i].position;
		vertices[i].normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertices[i].tex = mesh.vertices[i].TexC;
	}

	VertexBuffer* pVB = new VertexBuffer(gfx, vertices, L"GPU_Waves.");
	AddBind(pVB);

// 	pDynamicVB = new VertexBufferDynamic(gfx, vertices, L"GPU_Waves");
// 	pCopyDynamicVB = pDynamicVB->Get_p_DynamicVertexBuffer();
// 	AddBind(pDynamicVB);

	/*std::vector<UINT> indices(3 * (long long)wave.GetTriangleCount()); // 3 indices per face
	// Iterate over each quad.
	UINT m = wave.GetRowCount();
	UINT n = wave.GetColumnCount();
	int k = 0;
	for (UINT i = 0; i < m - 1; ++i)
	{
		for (DWORD j = 0; j < n - 1; ++j)
		{
			indices[k] = i * n + j;
			indices[(long long)k + 1] = i * n + j + 1;
			indices[(long long)k + 2] = (i + 1) * n + j;

			indices[(long long)k + 3] = (i + 1) * n + j;
			indices[(long long)k + 4] = i * n + j + 1;
			indices[(long long)k + 5] = (i + 1) * n + j + 1;

			k += 6; // next quad
		}
	}*/

	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(pTopology);

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, mesh.indices, L"WaveSurfaceIndexBuffer");
	AddIndexBuffer(pIndexBuffer);

	VertexConstantBuffer<CB_VS_Transform>* pVCB =
		new VertexConstantBuffer<CB_VS_Transform>(gfx, transformMatrices, 0u, 1u);
	pCopyVertexConstantBuffer = pVCB->GetVertexConstantBuffer();
	AddBind(pVCB);

	PixelShaderConstantBuffer<CB_PS_DirectionalL_Fog>* pPSCB =
		new PixelShaderConstantBuffer<CB_PS_DirectionalL_Fog>(gfx, directionalLight, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	AddBind(pPSCB);

	PixelShaderConstantBuffer<CB_PS_PerFrameUpdate>* pLightsCB =
		new PixelShaderConstantBuffer<CB_PS_PerFrameUpdate>(gfx, pscBuffer, 1u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pCopyPixelConstantBuffer = pLightsCB->GetPixelShaderConstantBuffer();
	AddBind(pLightsCB);

	std::wstring directory[1];
	directory[0] = L"Textures\\water2.dds";
	ShaderResourceView* pSRV = new ShaderResourceView(gfx, directory, (UINT)std::size(directory));
	AddBind(pSRV);

	TextureSampler* pTexSampler = new TextureSampler(gfx);
	AddBind(pTexSampler);


	//vertex shader clamp sampler
	/*D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 0.0f;

	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateSamplerState(&samplerDesc, &pVSSamplerClamp));*/

	//vertex shader linear wrap sampler
	D3D11_SAMPLER_DESC samplerDesc0;
	samplerDesc0.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc0.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc0.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc0.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc0.BorderColor[0] = 0.0f;
	samplerDesc0.BorderColor[1] = 0.0f;
	samplerDesc0.BorderColor[2] = 0.0f;
	samplerDesc0.BorderColor[3] = 0.0f;

	samplerDesc0.MipLODBias = 0.0f;
	samplerDesc0.MaxAnisotropy = 16;
	samplerDesc0.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc0.MinLOD = 0;
	samplerDesc0.MaxLOD = D3D11_FLOAT32_MAX;
	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateSamplerState(&samplerDesc0, &pVSSamplerWrap));


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

	gpuWavesCbuffer.waveConstant0 = wave.waveConstant[0];
	gpuWavesCbuffer.waveConstant1 = wave.waveConstant[1];
	gpuWavesCbuffer.waveConstant2 = wave.waveConstant[2];

	ComputeShaderConstantBuffer<CB_CS_GPUWaves>* pCSBuf = new ComputeShaderConstantBuffer<CB_CS_GPUWaves>(gfx, gpuWavesCbuffer,
		0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pCopyCScbuffer = pCSBuf->GetComputeShaderConstantBuffer();
	AddBind(pCSBuf);

}

void WaveSurfaceGPU::UpdateSolution(Graphics& gfx, float dt)
{
	time += dt;
// 	gfx.pgfx_pDeviceContext->VSSetSamplers(0u, 1u, &pVSSamplerClamp);
	gfx.pgfx_pDeviceContext->VSSetSamplers(0u, 1u, &pVSSamplerWrap);

	if (time >= wave.TimeStep())
	{
		gfx.pgfx_pDeviceContext->CSSetShaderResources(0u, 1u, &pPreviousSolutionSRV);
		gfx.pgfx_pDeviceContext->CSSetShaderResources(1u, 1u, &pCurrentSolutionSRV);
		gfx.pgfx_pDeviceContext->CSSetUnorderedAccessViews(0u, 1u, &pNextSolutionUAV, 0u);
		
		UINT numGroupsX = wave.GetColumnCount() / 16;
		UINT numGroupsY = wave.GetColumnCount() / 16;
		gfx.pgfx_pDeviceContext->Dispatch(numGroupsX, numGroupsY, 1);

		ID3D11UnorderedAccessView* nullUAV = nullptr;
		ID3D11ShaderResourceView* nullSRV = nullptr;
		gfx.pgfx_pDeviceContext->CSSetShaderResources(0u, 1u, &nullSRV);
// 		gfx.pgfx_pDeviceContext->CSSetShaderResources(1u, 1u, &nullSRV);
		gfx.pgfx_pDeviceContext->CSSetUnorderedAccessViews(0u, 1u, &nullUAV, 0u);

		ID3D11ShaderResourceView* srvTemp = pPreviousSolutionSRV;
		pPreviousSolutionSRV = pCurrentSolutionSRV;
		pCurrentSolutionSRV = pNextSolutionSRV;
		pNextSolutionSRV = srvTemp;

		ID3D11UnorderedAccessView* uavTemp = pPreviousSolutionUAV;
		pPreviousSolutionUAV = pCurrentSolutionUAV;
		pCurrentSolutionUAV = pNextSolutionUAV;
		pNextSolutionUAV = uavTemp;

		time = 0.0f;
	}
	gfx.pgfx_pDeviceContext->VSSetShaderResources(0u, 1u, &pCurrentSolutionSRV);
}

void WaveSurfaceGPU::Disturb(Graphics& gfx)
{
// 	ID3D11ShaderResourceView* nullSRV = nullptr;
// 	gfx.pgfx_pDeviceContext->VSSetShaderResources(0u, 1u, &nullSRV);


	unsigned long i = 5 + MathHelper::RandomIntWithingRange(0, INT_MAX) % (numColumns - 10);
	unsigned long j = 5 + MathHelper::RandomIntWithingRange(0, INT_MAX) % (numRows - 10);
	float magnitute = MathHelper::RandomFloatWithinRange(1.0f, 2.0f);
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyCScbuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	CB_CS_GPUWaves* data = reinterpret_cast<CB_CS_GPUWaves*> (mappedData.pData);
	data->disturbIndex[0] = i;
	data->disturbIndex[1] = j;
	data->disturbMagnitute = magnitute;
	gfx.pgfx_pDeviceContext->Unmap(pCopyCScbuffer, 0u);
	gfx.pgfx_pDeviceContext->CSSetUnorderedAccessViews(0u, 1u, &pCurrentSolutionUAV, 0u);
	gfx.pgfx_pDeviceContext->Dispatch(1, 1, 1);
	ID3D11UnorderedAccessView* nullUAV = nullptr;

	gfx.pgfx_pDeviceContext->CSSetUnorderedAccessViews(0u, 1u, &nullUAV, 0u);
// 	gfx.pgfx_pDeviceContext->VSSetShaderResources(0u, 1u, &pCurrentSolutionSRV);

}


void WaveSurfaceGPU::UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj)
{
	waterTextureOffset.y += 0.05f * alpha;
	waterTextureOffset.x += 0.1f * alpha;
	wavesOffset = DirectX::XMMatrixTranslation(waterTextureOffset.x, waterTextureOffset.y, 0.0f);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVertexConstantBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	CB_VS_Transform* object = reinterpret_cast<CB_VS_Transform*>(mappedData.pData);
	object->world = in_world;
	object->worldInvTranspose = MathHelper::InverseTranspose(in_world);
	object->worldViewProjection = DirectX::XMMatrixTranspose(in_world * in_ViewProj);
// 	object->texTransform = wavesScale * wavesOffset;
	object->texTransform = DirectX::XMMatrixIdentity();

	gfx.pgfx_pDeviceContext->Unmap(pCopyVertexConstantBuffer, 0u);


}

void WaveSurfaceGPU::UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPositon)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyPixelConstantBuffer, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CB_PS_PerFrameUpdate* frame = reinterpret_cast<CB_PS_PerFrameUpdate*> (mappedData.pData);
	frame->cameraPositon = camPositon;

	if (GetAsyncKeyState('0') & 0x8000)
		frame->numberOfLights = 0;
	if (GetAsyncKeyState('1') & 0x8000)
		frame->numberOfLights = 1;
	if (GetAsyncKeyState('2') & 0x8000)
		frame->numberOfLights = 2;
	if (GetAsyncKeyState('3') & 0x8000)
		frame->numberOfLights = 3;

	gfx.pgfx_pDeviceContext->Unmap(pCopyPixelConstantBuffer, 0u);
}

void WaveSurfaceGPU::ClearVertexShaderResource(Graphics& gfx)
{
	ID3D11ShaderResourceView* nullSRV = nullptr;
	gfx.pgfx_pDeviceContext->VSSetShaderResources(0u, 1u, &nullSRV);

}
