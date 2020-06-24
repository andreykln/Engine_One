#include "Hills.h"
#include <cmath>

Hills::Hills(Graphics& gfx, float in_width, float in_depth, UINT in_m, UINT in_n, bool isFlat)
	: width(in_width), depth(in_depth), m(in_m), n(in_n), flatSurface(isFlat)
{
// 	dirLight.ambient = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
// 	dirLight.diffuse = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
// 	dirLight.specular = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
// 	dirLight.direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
// 	dirLight.padding = 0.0f;

	constLights.dirLight[0].ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	constLights.dirLight[0].diffuse = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	constLights.dirLight[0].direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	constLights.dirLight[0].specular = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);

	constLights.dirLight[1].ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	constLights.dirLight[1].diffuse = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
	constLights.dirLight[1].direction = DirectX::XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);
	constLights.dirLight[1].specular = DirectX::XMFLOAT4(0.55f, 0.55f, 0.55f, 1.0f);

	constLights.dirLight[2].ambient = DirectX::XMFLOAT4(0.5, 0.5f, 0.5f, 1.0f);
	constLights.dirLight[2].diffuse = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
	constLights.dirLight[2].direction = DirectX::XMFLOAT3(0.0f, -0.707f, -0.707f);
	constLights.dirLight[2].specular = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);


	//point light .position will change every frame

// 	if (!flatSurface) 
// 	{
// 		pointLight.ambient = DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
// 		pointLight.diffuse = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
// 		pointLight.specular = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
// 		pointLight.attenuation = DirectX::XMFLOAT3(0.0f, 0.1f, 0.0f);
// 
// 
// 		pointLight.range = 25.0f;
// 		pointLight.padding = 0.0f;
// 		//position and direction will change every frame
// 		spotLight.ambient = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
// 		spotLight.diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
// 		spotLight.specular = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
// 		spotLight.attenuation = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
// 		spotLight.spot = 96.0f;
// 		spotLight.range = 10000.0f;
// 		spotLight.padding = 0.0f;
// 	}

	landMat.ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	landMat.diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	landMat.specular = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 32.0f);

// 	constLights.gDirLight = dirLight;
// 	constLights.gEyePosW = { 0.0f, 0.0f, 0.0f };
// 	constLights.gPointLight = pointLight;
// 	constLights.gSpotLight = spotLight;
// 	
// 	constLights.gMaterial = landMat;
// 	constLights.padding = 0.0f;
	struct Vertex_l
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 tex;
	};


	landscapeGenerated.CreateGrid(width, depth, m, n, grid);

	std::vector<Vertex_l> vertices(grid.vertices.size());

	if (!flatSurface)
	{
		for (size_t i = 0; i < grid.vertices.size(); ++i)
		{
			
			DirectX::XMFLOAT3 p = grid.vertices[i].position;
			p.y = GetHeight(p.x, p.z);
			vertices[i].pos = p;
			vertices[i].normal = GetHillNormal(p.x, p.z);
			vertices[i].tex = grid.vertices[i].TexC;
		}
	}
	else
	{
		for (size_t i = 0; i < grid.vertices.size(); ++i)
		{
			DirectX::XMFLOAT3 p = grid.vertices[i].position;
			vertices[i].pos = p;
			vertices[i].normal = DirectX::XMFLOAT3{ 0.0f, 1.0f, 0.0f};
			vertices[i].tex = grid.vertices[i].TexC;
		}

	}

	VertexBuffer* pVertexBuffer = new VertexBuffer(gfx, vertices, L"Hills");
	AddBind(pVertexBuffer);

	if (!flatSurface)
	{
		VertexShader* pVertexShader = new VertexShader(gfx, L"Shaders\\Vertex\\LightAndTextureVS.cso");
		pVertexShaderBlob = pVertexShader->GetByteCode();
		AddBind(pVertexShader);
	}
	else
	{
		VertexShader* pVertexShader = new VertexShader(gfx, L"Shaders\\Vertex\\LightAndTextureVS.cso");
		pVertexShaderBlob = pVertexShader->GetByteCode();
		AddBind(pVertexShader);
	}
		const std::vector<D3D11_INPUT_ELEMENT_DESC> inputElemDesc =
		{
			{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA, 0u},
			{"TexCoordinate", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA, 0u},
			{"Normal", 0u, DXGI_FORMAT_R8G8B8A8_UNORM, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA, 0u}
		};

	InputLayout* pInputLayout = new InputLayout(gfx, pVertexShaderBlob, inputElemDesc, L"PositionAndColor");
	AddBind(pInputLayout);



	if (!flatSurface)
	{
		PixelShader* pPixelShader = new PixelShader(gfx, L"Shaders\\Pixel\\LightAndTexturePS.cso");
		AddBind(pPixelShader);
	}
	else
	{
		PixelShader* pPixelShader = new PixelShader(gfx, L"Shaders\\Pixel\\LightAndTexturePS.cso");
		AddBind(pPixelShader);
	}

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, grid.indices, L"HillsIndexBuffer");
	AddIndexBuffer(pIndexBuffer);

	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(pTopology);


	VertexConstantBuffer<CBPerObjectTexture>* pPerObject =
		new VertexConstantBuffer<CBPerObjectTexture>(gfx, constMatrices,  0u, 1u);
	pCopyVCBMatricesHills = pPerObject->GetVertexConstantBuffer();
	AddBind(pPerObject);

	constLights.objectMaterial = landMat;
	PixelShaderConstantBuffer<CBPerFrame>* pPerFrameCB =
		new PixelShaderConstantBuffer<CBPerFrame>(gfx, constLights, 1u, 1u);
	pCopyPCBLightsHills = pPerFrameCB->GetPixelShaderConstantBuffer();
	AddBind(pPerFrameCB);

	ShaderResourceView* pSRV = new ShaderResourceView(gfx, L"Textures\\grass.dds");
	AddBind(pSRV);

// 	RasterizerState state;
// 	Rasterizer* pRasterState = new Rasterizer(gfx, state.Wireframe());
// 	AddBind(pRasterState);
}


DirectX::XMMATRIX Hills::GetTransform() const noexcept
{
	return m_Matrix * m_Centered;;
}

void Hills::Update(float dt) noexcept
{
	alpha = dt;
}

void Hills::SetWidth(float in_width) noexcept
{
	width = in_width;
}

void Hills::SetDepth(float in_depth) noexcept
{
	depth = in_depth;
}

DirectX::XMFLOAT3 Hills::GetHillNormal(float x, float z) const
{
	DirectX::XMFLOAT3 n(
		-0.03f * z * cosf(0.1f * x) - 0.3f * cosf(0.1f * z),
		1.0f,
		-0.3f * sinf(0.1f * x) + 0.03f * x * sinf(0.1f * z));
	DirectX::XMVECTOR unitNormal = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&n));
	DirectX::XMStoreFloat3(&n, unitNormal);
	return n;
}


void Hills::SetVerticesWidth(UINT in_vertWidth) noexcept
{
	m = in_vertWidth;
}

void Hills::SetVerticesDepth(UINT in_vertDepth) noexcept
{
	n = in_vertDepth;
}


void Hills::UpdateVertexConstantBuffer(Graphics& gfx)
{

}

void Hills::UpdateConstantBuffers(Graphics& gfx,
	DirectX::XMFLOAT3& eyePosition, DirectX::XMVECTOR& pos,
	DirectX::XMVECTOR& target)
{
 	using namespace DirectX;

	if (!flatSurface)
	{
		// Circle light over the land surface.
		pointLight.position.x = 70.0f * std::cosf(0.2f * GetAlpha());
		pointLight.position.z = 70.0f * std::sinf(0.2f * GetAlpha());
		pointLight.position.y = MathHelper::Max(GetHeight(pointLight.position.x, pointLight.position.z), -3.0f) + 10.0f;
		// The spotlight takes on the camera position and is aimed in the
		// same direction the camera is looking.  
		spotLight.position = eyePosition;
// 		DirectX::XMVECTOR tetEyeTarget = { eyePosition.x, eyePosition.y, eyePosition.z, 1.0f };
// 		DirectX::XMVECTOR test_target = { 60.0f,	-70.0f,	70.0f, 1.0f };
		DirectX::XMVECTOR flashLightvector = -DirectX::XMVector3Normalize(target - pos);
		//to move light a bit up
		flashLightvector.m128_f32[0] += 0.2f;
		flashLightvector.m128_f32[2] += 0.03f;

		DirectX::XMStoreFloat3(&spotLight.direction, flashLightvector);

		D3D11_MAPPED_SUBRESOURCE mappedData;
		DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVCBMatricesHills, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
		CBPerObjectTexture* object = reinterpret_cast<CBPerObjectTexture*>(mappedData.pData);
		object->gWorld = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
		object->gWorldInvTranspose = MathHelper::InverseTranspose(object->gWorld);
		object->gWorldViewProj = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
		object->gTexTransform = grassScaling;
		gfx.pgfx_pDeviceContext->Unmap(pCopyVCBMatricesHills, 0u);

		DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyPCBLightsHills, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
		CBPerFrame* frame = reinterpret_cast<CBPerFrame*> (mappedData.pData);
		if (GetAsyncKeyState('0') & 0x8000)
			frame->numLights = 0;
		if (GetAsyncKeyState('1') & 0x8000)
			frame->numLights = 1;
		if (GetAsyncKeyState('2') & 0x8000)
			frame->numLights = 2;
		if (GetAsyncKeyState('3') & 0x8000)
			frame->numLights = 3;
		//frame->objectMaterial = landMat;
		frame->objectMaterial = landMat;
		gfx.pgfx_pDeviceContext->Unmap(pCopyPCBLightsHills, 0u);
		//SPOTLIGHT HERE
// 		DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyPCBLightsHills, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
// 		CBPerFrame* frame = reinterpret_cast<CBPerFrame*>(mappedData.pData);
// 		frame->gPointLight.position.x = pointLight.position.x;
// 		frame->gPointLight.position.y = pointLight.position.y;
// 		frame->gPointLight.position.z = pointLight.position.z;
// 		frame->gSpotLight.position = spotLight.position;
// 		frame->gSpotLight.direction = spotLight.direction;
// 		gfx.pgfx_pDeviceContext->Unmap(pCopyPCBLightsHills, 0u);
	}
	else
	{
		D3D11_MAPPED_SUBRESOURCE mappedData;
		DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVCBMatricesHills, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
		CBPerObject* object = reinterpret_cast<CBPerObject*>(mappedData.pData);
		object->gWorld = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
		object->gWorldInvTranspose = MathHelper::InverseTranspose(object->gWorld);
		object->gWorldViewProj = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
		gfx.pgfx_pDeviceContext->Unmap(pCopyVCBMatricesHills, 0u);

		DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyPCBLightsHills, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
		CBPerFrame* frame = reinterpret_cast<CBPerFrame*> (mappedData.pData);
		if (GetAsyncKeyState('0') & 0x8000)
			frame->numLights = 0;
		if (GetAsyncKeyState('1') & 0x8000)
			frame->numLights = 1;
		if (GetAsyncKeyState('2') & 0x8000)
			frame->numLights = 2;
		if (GetAsyncKeyState('3') & 0x8000)
			frame->numLights = 3;
		//frame->objectMaterial = landMat;
		frame->objectMaterial = landMat;
		gfx.pgfx_pDeviceContext->Unmap(pCopyPCBLightsHills, 0u);
 	}
}


float Hills::GetAlpha() const noexcept
{
	return alpha;
}

float Hills::GetHeight(float x, float z) const
{
	return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z));
}
