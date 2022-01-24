#include "GaussianBlur.h"

GaussianBlur::GaussianBlur(Graphics& gfx)
{
	geoGen.CreateBlurQuad(mesh);
	
// 	VertexBuffer* pVB = new VertexBuffer(gfx, mesh.vertices, L"BlurQuadVertices");
// 	AddBind(pVB);
// 
// 	IndexBuffer* pIB = new IndexBuffer(gfx, mesh.indices, L"BlurIndicex");
// 	AddIndexBuffer(pIB);
// 
// 	Topology* pT = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
// 	AddBind(pT);
// 
// 	VertexConstantBuffer<CB_VS_Transform>* pVCBPerObject =
// 		new VertexConstantBuffer<CB_VS_Transform>(gfx, transformMatrices, 0u, 1u);
// 	pCopyVCBBlur = pVCBPerObject->GetVertexConstantBuffer();
// 	AddBind(pVCBPerObject);



	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = resolution_width;
	texDesc.Height = resolution_height;
	texDesc.MipLevels = 1u;
	texDesc.ArraySize = 1u;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	gfx.pgfx_pDevice->CreateTexture2D(&texDesc, 0u, &pTextureToBlur);
	//null description means to create view to all mipmap levels using
	// the format the texture was created with
	gfx.pgfx_pDevice->CreateRenderTargetView(pTextureToBlur, 0u, &pRTVtoBlur);
	gfx.pgfx_pDevice->CreateUnorderedAccessView(pTextureToBlur, 0u, &pUAV);
	gfx.pgfx_pDevice->CreateShaderResourceView(pTextureToBlur, 0u, &pSRV);
	pTextureToBlur->Release();
	
// 	TextureSampler* pTexSampler = new TextureSampler(gfx, ShaderType::Pixel);
// 	AddBind(pTexSampler);


	D3D11_TEXTURE2D_DESC blurredTexDesc;
	blurredTexDesc.Width = resolution_width;
	blurredTexDesc.Height = resolution_height;
	blurredTexDesc.MipLevels = 1;
	blurredTexDesc.ArraySize = 1;
	blurredTexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	blurredTexDesc.SampleDesc.Count = 1;
	blurredTexDesc.SampleDesc.Quality = 0;
	blurredTexDesc.Usage = D3D11_USAGE_DEFAULT;
	blurredTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	blurredTexDesc.CPUAccessFlags = 0;
	blurredTexDesc.MiscFlags = 0;
	ID3D11Texture2D* blurredTex = nullptr;
	gfx.pgfx_pDevice->CreateTexture2D(&blurredTexDesc, 0u, &blurredTex);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1u;
	srvDesc.Texture2D.MostDetailedMip = 0u;
	gfx.pgfx_pDevice->CreateShaderResourceView(blurredTex, &srvDesc, &pBlurredOutputSRV);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = 0;
	gfx.pgfx_pDevice->CreateUnorderedAccessView(blurredTex, &uavDesc, &pBlurredOutputUAV);

}

ID3D11RenderTargetView* GaussianBlur::GetRTV() const
{
	return pRTVtoBlur;
}

void GaussianBlur::PerformHorizontalBlur(Graphics& gfx)
{

	gfx.pgfx_pDeviceContext->CSSetShaderResources(0u, 1u, &pSRV);
	gfx.pgfx_pDeviceContext->CSSetUnorderedAccessViews(0u, 1u, &pBlurredOutputUAV, 0u);

	UINT numGroupsX = (UINT)(ceil(resolution_width / 256.0f));
	gfx.pgfx_pDeviceContext->Dispatch(numGroupsX, resolution_height, 1);

	ID3D11UnorderedAccessView* nullUAV = nullptr;
	ID3D11ShaderResourceView* nullSRV = nullptr;
	gfx.pgfx_pDeviceContext->CSSetShaderResources(0u, 1u, &nullSRV);
	gfx.pgfx_pDeviceContext->CSSetUnorderedAccessViews(0u, 1u, &nullUAV, 0u);

}

void GaussianBlur::PerformVerticalBlur(Graphics& gfx)
{
	gfx.pgfx_pDeviceContext->CSSetShaderResources(0u, 1u, &pBlurredOutputSRV);
	gfx.pgfx_pDeviceContext->CSSetUnorderedAccessViews(0u, 1u, &pUAV, 0u);

	UINT numGroupsY = (UINT)(ceil(resolution_height / 256.0f));
	gfx.pgfx_pDeviceContext->Dispatch(resolution_width, numGroupsY, 1);

	ID3D11UnorderedAccessView* nullUAV = nullptr;
	ID3D11ShaderResourceView* nullSRV = nullptr;
	gfx.pgfx_pDeviceContext->CSSetShaderResources(0u, 1u, &nullSRV);
	gfx.pgfx_pDeviceContext->CSSetUnorderedAccessViews(0u, 1u, &nullUAV, 0u);
	gfx.pgfx_pDeviceContext->PSSetShaderResources(0u, 1u, &pSRV);
}

ID3D11Texture2D* GaussianBlur::GetBlurredtexture()
{
	return pTextureToBlur;
}

void GaussianBlur::UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVCBBlur, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	CB_VS_Transform* pMatrices = reinterpret_cast<CB_VS_Transform*>(mappedData.pData);
	pMatrices->world = DirectX::XMMatrixIdentity();
	pMatrices->worldInvTranspose = DirectX::XMMatrixIdentity();
	pMatrices->worldViewProjection = DirectX::XMMatrixIdentity();
	pMatrices->texTransform = DirectX::XMMatrixIdentity();
	gfx.pgfx_pDeviceContext->Unmap(pCopyVCBBlur, 0u);
}

