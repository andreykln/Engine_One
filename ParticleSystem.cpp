#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(Graphics& gfx, UINT maxParticles)
	: maxParticles(maxParticles)
{

	// The initial particle emitter has type 0 and age 0.  The rest
// of the particle attributes do not apply to an emitter.
	Particle p;
	//Vertex buffer for Stream-Out
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(Particle) * 1; //why mul by 1?
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0u;
	vbd.MiscFlags = 0u;
	vbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA initDataParticle;
	initDataParticle.pSysMem = &p;

	gfx.pgfx_pDevice->CreateBuffer(&vbd, &initDataParticle, &pInitVB);
	
	//ping-pong buffers for stream out and drawing
	vbd.ByteWidth = sizeof(Particle) * maxParticles;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
	gfx.pgfx_pDevice->CreateBuffer(&vbd, 0u, &pStreamOutVB);
	gfx.pgfx_pDevice->CreateBuffer(&vbd, 0u, &pDrawVB);

	randomTexSRV = CreateRandomTexture1DSRV(gfx);
	


	TextureSampler* pGSSOsampler = new TextureSampler(gfx, ShaderType::Geometry);
	pSSGSSO = pGSSOsampler->GetSamplerState();

	//this ptr used only in manual binding, so release pointer 
	GSSOparticleFireData.emitterPositon = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	GSSOparticleFireData.gameTime = 0.0f;
	GSSOparticleFireData.timeStep = 0.0f;
	GeometryShaderConstantBuffer<CB_GS_StreamOut>* pGSCBSO = new GeometryShaderConstantBuffer<CB_GS_StreamOut>(gfx, GSSOparticleFireData,
		0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pGSSOParticleFire = pGSCBSO->GetGeometryShaderConstantBuffer();
	pGSCBSO = nullptr;

	GeometryShaderConstantBuffer<CB_CameraPosition_ViewProj>* pGSCBDraw =
		new GeometryShaderConstantBuffer<CB_CameraPosition_ViewProj>(gfx, camPosVP,
		0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pGSParticleFireDraw = std::move(pGSCBDraw->GetGeometryShaderConstantBuffer());
	pGSCBDraw = nullptr;

	const wchar_t* filePath = L"Textures\\flame.dds";
	ShaderResourceView* pSRV = new ShaderResourceView(gfx, filePath);
	psFireDrawTexture = pSRV->GetSRV();
	TextureSampler* pTexSampler = new TextureSampler(gfx, ShaderType::Pixel);
	pSSDrawPixel = pTexSampler->GetSamplerState();

}

void ParticleSystem::UpdateStreamOutConstBuffer(Graphics& gfx, DirectX::XMFLOAT3 emitPos, float timeStep, float gameTime)
{
	gfx.pgfx_pDeviceContext->GSSetConstantBuffers(0u, 1u, &pGSSOParticleFire);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pGSSOParticleFire, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CB_GS_StreamOut* StreamOut = reinterpret_cast<CB_GS_StreamOut*>(mappedData.pData);
	StreamOut->emitterPositon = emitPos;
	StreamOut->gameTime = gameTime;
	StreamOut->timeStep = timeStep;
	gfx.pgfx_pDeviceContext->Unmap(pGSSOParticleFire, 0u);
}

void ParticleSystem::UpdateParticleDrawConstBuffer(Graphics& gfx, DirectX::XMMATRIX viewProjection, DirectX::XMFLOAT3 cameraPos)
{
	gfx.pgfx_pDeviceContext->GSSetConstantBuffers(0u, 1u, &pGSParticleFireDraw);


	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pGSParticleFireDraw, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CB_CameraPosition_ViewProj* drawParticleGS = reinterpret_cast<CB_CameraPosition_ViewProj*>(mappedData.pData);
	drawParticleGS->cameraPosition = cameraPos;
	drawParticleGS->viewProjection = DirectX::XMMatrixTranspose(viewProjection);
	gfx.pgfx_pDeviceContext->Unmap(pGSParticleFireDraw, 0u);
}

void ParticleSystem::SetVertexBuffersAndDrawParticles(Graphics& gfx, Shaders* pShaders,
	DirectX::XMMATRIX viewProjection, DirectX::XMFLOAT3 cameraPos,
	DirectX::XMFLOAT3 emitPos, float timeStep, float gameTime)
{
	pShaders->BindVSandIA(ShaderPicker::Particles_StreamOut_VS_GS);

	UINT stride = sizeof(Particle);
	UINT offset = 0;
	if (firstRun)
	{
		BindToSOStage(gfx);
		UpdateStreamOutConstBuffer(gfx, emitPos, timeStep, gameTime);
		gfx.pgfx_pDeviceContext->IASetVertexBuffers(0u, 1u, &pInitVB, &stride, &offset);
		gfx.pgfx_pDeviceContext->GSSetSamplers(0u, 1u, &pSSGSSO);
	}
	else
	{
		BindToSOStage(gfx);
		UpdateStreamOutConstBuffer(gfx, emitPos, timeStep, gameTime);
		gfx.pgfx_pDeviceContext->GSSetSamplers(0u, 1u, &pSSGSSO);
		gfx.pgfx_pDeviceContext->IASetVertexBuffers(0u, 1u, &pDrawVB, &stride, &offset);
	}

	gfx.pgfx_pDeviceContext->SOSetTargets(1u, &pStreamOutVB, &offset);

	if (firstRun)
	{
		pShaders->BindVSandIA(ShaderPicker::Particles_StreamOut_VS_GS);
		pShaders->BindGS(ShaderPicker::Particles_StreamOut_VS_GS);
		gfx.pgfx_pDeviceContext->Draw(1u, 0u);
		firstRun = false;
	}
	else
	{
		pShaders->BindVSandIA(ShaderPicker::Particles_StreamOut_VS_GS);
		pShaders->BindGS(ShaderPicker::Particles_StreamOut_VS_GS);


		gfx.pgfx_pDeviceContext->DrawAuto();
	}


	// done streaming-out--unbind the vertex buffer
	UnbindFromSOStage(gfx);
	std::swap(pDrawVB, pStreamOutVB);


	gfx.pgfx_pDeviceContext->IASetVertexBuffers(0, 1, &pDrawVB, &stride, &offset);

	UpdateParticleDrawConstBuffer(gfx, viewProjection, cameraPos);
	// Draw the updated particle system we just streamed-out.
	pShaders->BindVSandIA(ShaderPicker::Particles_Draw_VS_GS_PS);
	pShaders->BindGS(ShaderPicker::Particles_Draw_VS_GS_PS);
	pShaders->BindPS(ShaderPicker::Particles_Draw_VS_GS_PS);
	gfx.pgfx_pDeviceContext->PSSetShaderResources(0u, 1u, &psFireDrawTexture);

	gfx.pgfx_pDeviceContext->PSSetSamplers(0u, 1u, &pSSDrawPixel); //second call of it, does it needed?
	gfx.pgfx_pDeviceContext->PSSetShaderResources(0u, 1u, &psFireDrawTexture);

	gfx.pgfx_pDeviceContext->DrawAuto();
	

}

void ParticleSystem::BindToSOStage(Graphics& gfx)
{
	gfx.pgfx_pDeviceContext->SOSetTargets(1u, &pStreamOutVB, 0u);
	gfx.pgfx_pDeviceContext->GSSetShaderResources(0u, 1u, &randomTexSRV);
}

void ParticleSystem::UnbindFromSOStage(Graphics& gfx)
{
	ID3D11Buffer* bufferArray[1] = { 0 };
	gfx.pgfx_pDeviceContext->SOSetTargets(1u, bufferArray, 0u);

}

ID3D11ShaderResourceView* ParticleSystem::CreateRandomTexture1DSRV(Graphics& gfx)
{
	DirectX::XMFLOAT4 *randomValues = new DirectX::XMFLOAT4[1024];

	for (int i = 0; i < 1024; ++i)
	{
		randomValues[i].x = MathHelper::RandomFloatWithinRange(-1.0f, 1.0f);
		randomValues[i].y = MathHelper::RandomFloatWithinRange(-1.0f, 1.0f);
		randomValues[i].z = MathHelper::RandomFloatWithinRange(-1.0f, 1.0f);
		randomValues[i].w = MathHelper::RandomFloatWithinRange(-1.0f, 1.0f);
	}

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = randomValues;
	initData.SysMemPitch = 1024 * sizeof(DirectX::XMFLOAT4); //it suppose to have no meaning for 1D texture
	initData.SysMemSlicePitch = 0u;

	D3D11_TEXTURE1D_DESC texDesc;
	texDesc.Width = 1024;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1u;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0u;
	texDesc.MiscFlags = 0u;

	ID3D11Texture1D* pRandomTex = nullptr;
	gfx.pgfx_pDevice->CreateTexture1D(&texDesc, &initData, &pRandomTex);

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	viewDesc.Texture1D.MipLevels = texDesc.MipLevels;
	viewDesc.Texture1D.MostDetailedMip = 0;

	ID3D11ShaderResourceView* pSRV = nullptr;
	gfx.pgfx_pDevice->CreateShaderResourceView(pRandomTex, &viewDesc, &pSRV);

	delete[] randomValues;
	return pSRV;
}

