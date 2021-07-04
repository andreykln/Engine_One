#include "DynamicCubeMap.h"

DynamicCubeMap::DynamicCubeMap(Graphics& gfx)
{
	//cubemap texture array
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = cubeMapSize;
	texDesc.Height = cubeMapSize;
	texDesc.MipLevels = 0;
	texDesc.ArraySize = 6;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;

	ID3D11Texture2D* cubeTex = nullptr;

	gfx.pgfx_pDevice->CreateTexture2D(&texDesc, 0u, &cubeTex);

	//RTV to each cube face
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	rtvDesc.Texture2DArray.ArraySize = 1;
	rtvDesc.Texture2DArray.MipSlice = 0;

	for (int i = 0; i < 6; ++i)
	{
		rtvDesc.Texture2DArray.FirstArraySlice = i;
		gfx.pgfx_pDevice->CreateRenderTargetView(cubeTex, &rtvDesc, &pDynamicCubeMapRTV[i]);
	}

	//SRV to whole cube map
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = -1;

	gfx.pgfx_pDevice->CreateShaderResourceView(cubeTex, &srvDesc, &pDynamicCubeMapSRV);

	cubeTex->Release();

	//depth texture for rendering the scene into the cubemap
	//that has the same resolution as the cubemap faces

	D3D11_TEXTURE2D_DESC depthTexDesc;
	depthTexDesc.Width = cubeMapSize;
	depthTexDesc.Height = cubeMapSize;
	depthTexDesc.MipLevels = 1;
	depthTexDesc.ArraySize = 1;
	depthTexDesc.SampleDesc.Count = 1;
	depthTexDesc.SampleDesc.Quality = 0;
	depthTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTexDesc.CPUAccessFlags = 0;
	depthTexDesc.MiscFlags = 0;

	ID3D11Texture2D* depthTex = nullptr;
	gfx.pgfx_pDevice->CreateTexture2D(&depthTexDesc, 0u, &depthTex);

	//depth stencil view for the entire cube
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Format = depthTexDesc.Format;
	dsvDesc.Flags = 0;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	gfx.pgfx_pDevice->CreateDepthStencilView(depthTex, &dsvDesc, &pDynamicCubeMapDSV);

	depthTex->Release();

	dynamicCubeMapViewport.TopLeftX = 0.0f;
	dynamicCubeMapViewport.TopLeftY = 0.0f;
	dynamicCubeMapViewport.Width = (float)cubeMapSize;
	dynamicCubeMapViewport.Height = (float)cubeMapSize;
	dynamicCubeMapViewport.MinDepth = 0.0f;
	dynamicCubeMapViewport.MaxDepth = 1.0f;


}

void DynamicCubeMap::BuildCubeFaceCamera(float x, float y, float z)
{
	// Generate the cube map about the given position.
	DirectX::XMFLOAT3 center(x, y, z);
	DirectX::XMFLOAT3 worldUp(0.0f, 1.0f, 0.0f);

	// Look along each coordinate axis.
	DirectX::XMFLOAT3 targets[6] =
	{
		DirectX::XMFLOAT3(x + 1.0f, y, z), // +X
		DirectX::XMFLOAT3(x - 1.0f, y, z), // -X
		DirectX::XMFLOAT3(x, y + 1.0f, z), // +Y
		DirectX::XMFLOAT3(x, y - 1.0f, z), // -Y
		DirectX::XMFLOAT3(x, y, z + 1.0f), // +Z
		DirectX::XMFLOAT3(x, y, z - 1.0f)  // -Z
	};

	// Use world up vector (0,1,0) for all directions except +Y/-Y.  In these cases, we
	// are looking down +Y or -Y, so we need a different "up" vector.
	DirectX::XMFLOAT3 ups[6] =
	{
		DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),  // +X
		DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),  // -X
		DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f), // +Y
		DirectX::XMFLOAT3(0.0f, 0.0f, +1.0f), // -Y
		DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),  // +Z
		DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)	 // -Z
	};

	for (int i = 0; i < 6; ++i)
	{
		dynamicCubeCamera[i].SetLookAt(&center, &ups[i], &targets[i]);
		cubeFaceProjection[i] = dynamicCubeCamera[i].GetViewProjectionCubeMatrix(1.0f, 0.1f);

	//textbook camera, keep it in case
// 		DCcamera[i].LookAt(center, targets[i], ups[i]);
// 		DCcamera[i].SetLens(0.5f * XM_PI, 1.0f, 0.1f, 1000.0f);
// 		DCcamera[i].UpdateViewMatrix();
	}


}

D3D11_VIEWPORT DynamicCubeMap::GetViewPort()
{
	return dynamicCubeMapViewport;
}

ID3D11DepthStencilView* DynamicCubeMap::GetCubeMapDSV()
{
	return pDynamicCubeMapDSV;
}

ID3D11ShaderResourceView* DynamicCubeMap::GetCubeMapSRV()
{
	return pDynamicCubeMapSRV;
}
