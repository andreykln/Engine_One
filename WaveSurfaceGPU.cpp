#include "WaveSurfaceGPU.h"

WaveSurfaceGPU::WaveSurfaceGPU(Graphics& gfx)
{
	wave.Initialize(256, 256, 0.8f, 0.03f, 3.25f, 0.4f);

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

	vertices.resize(wave.GetVertexCount());


	VertexBuffer* pVB = new VertexBuffer(gfx, vertices, L"GPU_Waves.");
	AddBind(pVB);

// 	pDynamicVB = new VertexBufferDynamic(gfx, vertices, L"GPU_Waves");
// 	pCopyDynamicVB = pDynamicVB->Get_p_DynamicVertexBuffer();
// 	AddBind(pDynamicVB);

	std::vector<UINT> indices(3 * (long long)wave.GetTriangleCount()); // 3 indices per face
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
	}

	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(pTopology);

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, indices, L"WaveSurfaceIndexBuffer");
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
	D3D11_SAMPLER_DESC samplerDesc;
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
	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateSamplerState(&samplerDesc, &pVSSamplerClamp));

	//vertex shader linear wrap sampler
	D3D11_SAMPLER_DESC samplerDesc0;
	samplerDesc0.Filter = D3D11_FILTER_ANISOTROPIC;
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
	texDesc.Width = 256;
	texDesc.Height = 256;
	texDesc.MipLevels = 1u;
	texDesc.ArraySize = 1u;
	texDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_TEXTURE2D_DESC texDesc0;
	texDesc0.Width = 256;
	texDesc0.Height = 256;
	texDesc0.MipLevels = 1u;
	texDesc0.ArraySize = 1u;
	texDesc0.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	texDesc0.SampleDesc.Count = 1;
	texDesc0.SampleDesc.Quality = 0;
	texDesc0.Usage = D3D11_USAGE_DEFAULT;
	texDesc0.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	texDesc0.CPUAccessFlags = 0;
	texDesc0.MiscFlags = 0;

	D3D11_TEXTURE2D_DESC texDesc1;
	texDesc1.Width = 256;
	texDesc1.Height = 256;
	texDesc1.MipLevels = 1u;
	texDesc1.ArraySize = 1u;
	texDesc1.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	texDesc1.SampleDesc.Count = 1;
	texDesc1.SampleDesc.Quality = 0;
	texDesc1.Usage = D3D11_USAGE_DEFAULT;
	texDesc1.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	texDesc1.CPUAccessFlags = 0;
	texDesc1.MiscFlags = 0;

	gfx.pgfx_pDevice->CreateTexture2D(&texDesc, 0u, &pCurrSolution);
	gfx.pgfx_pDevice->CreateTexture2D(&texDesc0, 0u, &pPrevSolution);
	gfx.pgfx_pDevice->CreateTexture2D(&texDesc1, 0u, &pOutput);

	gfx.pgfx_pDevice->CreateShaderResourceView(pCurrSolution, 0u, &pCurrSolutionSRV);
	gfx.pgfx_pDevice->CreateShaderResourceView(pPrevSolution, 0u, &pPrevSolutionSRV);

// 	gfx.pgfx_pDevice->CreateUnorderedAccessView(pCurrSolution, 0u, &pCurrSolutionUAV);
// 	gfx.pgfx_pDevice->CreateUnorderedAccessView(pPrevSolution, 0u, &pPrevSolutionUAV);
	gfx.pgfx_pDevice->CreateUnorderedAccessView(pOutput, 0u, &pOutputUAV);

// 	gfx.pgfx_pDevice->CreateShaderResourceView(pCurrSolution, 0u, &pCurrSolutionSRV);
// 	gfx.pgfx_pDevice->CreateShaderResourceView(pPrevSolution, 0u, &pPrevSolutionSRV);
	gfx.pgfx_pDevice->CreateShaderResourceView(pOutput, 0u, &pOutputSRV);


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
	gfx.pgfx_pDeviceContext->VSSetSamplers(0u, 1u, &pVSSamplerClamp);
	gfx.pgfx_pDeviceContext->VSSetSamplers(1u, 1u, &pVSSamplerWrap);

// 	if (time >= wave.timeStep)
// 	{
		gfx.pgfx_pDeviceContext->CSSetShaderResources(0u, 1u, &pPrevSolutionSRV);
		gfx.pgfx_pDeviceContext->CSSetShaderResources(1u, 1u, &pCurrSolutionSRV);
		gfx.pgfx_pDeviceContext->CSSetUnorderedAccessViews(0u, 1u, &pOutputUAV, 0u);
		

		UINT numGroupsX = wave.GetColumnCount() / 16;
		UINT numGroupsY = wave.GetRowCount() / 16;
		gfx.pgfx_pDeviceContext->Dispatch(numGroupsX, numGroupsY, 1);

		ID3D11UnorderedAccessView* nullUAV = nullptr;
		ID3D11ShaderResourceView* nullSRV = nullptr;
		gfx.pgfx_pDeviceContext->CSSetShaderResources(0u, 1u, &nullSRV);
		gfx.pgfx_pDeviceContext->CSSetShaderResources(1u, 1u, &nullSRV);
		gfx.pgfx_pDeviceContext->CSSetUnorderedAccessViews(0u, 1u, &nullUAV, 0u);


		gfx.pgfx_pDeviceContext->VSSetShaderResources(0u, 1u, &pOutputSRV);

		auto srvTemp = pCurrSolutionSRV;
		pCurrSolutionSRV = pPrevSolutionSRV;
		pPrevSolutionSRV = srvTemp;
// 		pOutputUAV = uavTemp;

// 		time = 0.0f;
// 	}


		

}

void WaveSurfaceGPU::Disturb(Graphics& gfx)
{
	unsigned long i = 5 + MathHelper::RandomIntWithingRange(0, INT_MAX) % (wave.GetRowCount() - 10);
	unsigned long j = 5 + MathHelper::RandomIntWithingRange(0, INT_MAX) % (wave.GetColumnCount() - 10);
	float magnitute = MathHelper::RandomFloatWithinRange(1.0f, 2.0f);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyCScbuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	CB_CS_GPUWaves* data = reinterpret_cast<CB_CS_GPUWaves*> (mappedData.pData);
	data->disturbIndex[0] = i;
	data->disturbIndex[1] = j;
	data->disturbMagnitute = magnitute;
	gfx.pgfx_pDeviceContext->Unmap(pCopyCScbuffer, 0u);
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
