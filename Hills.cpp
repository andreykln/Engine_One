#include "Hills.h"
#include <cmath>

Hills::Hills(Graphics& gfx, float in_width, float in_depth, UINT in_m, UINT in_n, bool isFlat)
	: width(in_width), depth(in_depth), m(in_m), n(in_n), flatSurface(isFlat)
{
	dirLight.ambient = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	dirLight.diffuse = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	dirLight.specular = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	dirLight.direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	dirLight.padding = 0.0f;
	//point light .position will change every frame
	pointLight.ambient = DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	pointLight.diffuse = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	pointLight.specular = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	pointLight.attenuation = DirectX::XMFLOAT3(0.0f, 0.1f, 0.0f);


	pointLight.range = 25.0f;
	pointLight.padding = 0.0f;
	//position and direction will change every frame
	spotLight.ambient = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	spotLight.diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	spotLight.specular = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	spotLight.attenuation = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	spotLight.spot = 96.0f;
	spotLight.range = 10000.0f;
	spotLight.padding = 0.0f;
	landMat.ambient = DirectX::XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
	landMat.diffuse = DirectX::XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
	landMat.specular = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);
	
	wavesMat.ambient = DirectX::XMFLOAT4(0.137f, 0.42f, 0.556f, 1.0f);
	wavesMat.diffuse = DirectX::XMFLOAT4(0.137f, 0.42f, 0.556f, 1.0f);
	wavesMat.specular = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 96.0f);

	perFrameConstBuff.gDirLight = dirLight;
	perFrameConstBuff.gEyePosW = { 0.0f, 0.0f, 0.0f };
	perFrameConstBuff.gPointLight = pointLight;
	perFrameConstBuff.gSpotLight = spotLight;
	
	perFrameConstBuff.gMaterial = landMat;
	perFrameConstBuff.padding = 0.0f;
	struct Vertex_l
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
	};


	landscapeGenerated.CreateGrid(width, depth, m, n, grid);
	std::vector<Vertex_l> vertices(grid.vertices.size());
	
	for (size_t i = 0; i < grid.vertices.size(); ++i) 
	{
		DirectX::XMFLOAT3 p = grid.vertices[i].position;
		if (!flatSurface)
		{
			p.y = GetHeight(p.x, p.z);
		}
		vertices[i].pos = p;
		vertices[i].normal = GetHillNormal(p.x, p.z);
	}

	VertexBuffer* pVertexBuffer = new VertexBuffer(gfx, vertices, L"Hills");
	AddBind(pVertexBuffer);

	VertexShader* pVertexShader = new VertexShader(gfx, L"HillsLightVS.cso");
	ID3DBlob* pVertexShaderBlob = pVertexShader->GetByteCode();
	AddBind(pVertexShader);

	const std::vector<D3D11_INPUT_ELEMENT_DESC> inputElemDesc =
	{
		{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"Normal", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u}
	};

	InputLayout* pInputLayout = new InputLayout(gfx, pVertexShaderBlob, inputElemDesc, L"PositionAndColor");
	AddBind(pInputLayout);


	PixelShader* pPixelShader = new PixelShader(gfx, L"HillsLightPS.cso");
	AddBind(pPixelShader);

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, grid.indices, L"HillsIndexBuffer");
	AddIndexBuffer(pIndexBuffer);

	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(pTopology);


	VertexConstantBuffer<PerObject>* pPerObject =
		new VertexConstantBuffer<PerObject>(gfx, perObjectConstBuff,  0u, 1u);
	pCopyVCBPerObject = pPerObject->GetVertexConstantBuffer();
	AddBind(pPerObject);


	PixelShaderConstantBuffer<PerFrame>* pPerFrameCB =
		new PixelShaderConstantBuffer<PerFrame>(gfx, perFrameConstBuff, 1u, 1u);
	pCopyPCBPerFrame = pPerFrameCB->GetPixelShaderConstantBuffer();
	AddBind(pPerFrameCB);


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
	DirectX::XMVECTOR& target, DirectX::XMMATRIX world)
{
 	using namespace DirectX;
	// Circle light over the land surface.
	pointLight.position.x = 70.0f * std::cosf(0.2f * GetAlpha());
	pointLight.position.z = 70.0f * std::sinf(0.2f * GetAlpha());
	pointLight.position.y = MathHelper::Max(GetHeight(pointLight.position.x, pointLight.position.z), -3.0f) + 10.0f;
	// The spotlight takes on the camera position and is aimed in the
// same direction the camera is looking.  
	spotLight.position = eyePosition;
	DirectX::XMVECTOR test_target = { 70.0f * std::cosf(0.2f * GetAlpha()) ,
	-70.0f * std::cosf(0.2f * GetAlpha()) ,
	70.0f * std::cosf(0.2f * GetAlpha()) ,
	1.0f};
	DirectX::XMStoreFloat3(&spotLight.direction, DirectX::XMVector3Normalize(test_target - pos));

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVCBPerObject, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	PerObject* object = reinterpret_cast<PerObject*>(mappedData.pData);
	//DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection())
	object->gWorld = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection() );
	object->gWorldInvTranspose = MathHelper::InverseTranspose(object->gWorld);
	object->gWorldViewProj = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
	gfx.pgfx_pDeviceContext->Unmap(pCopyVCBPerObject, 0u);

	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyPCBPerFrame, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	PerFrame* frame = reinterpret_cast<PerFrame*>(mappedData.pData);
	frame->gPointLight.position.x = pointLight.position.x;
	frame->gPointLight.position.y = pointLight.position.y;
	frame->gPointLight.position.z = pointLight.position.z;


	frame->gSpotLight.position = spotLight.position;
	frame->gSpotLight.direction = spotLight.direction;
 	frame->gEyePosW = eyePosition;
	
	gfx.pgfx_pDeviceContext->Unmap(pCopyPCBPerFrame, 0u);




}


float Hills::GetAlpha() const noexcept
{
	return alpha;
}

float Hills::GetHeight(float x, float z) const
{
	return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z));
}
