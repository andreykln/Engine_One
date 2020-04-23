#include "ShapesDemo.h"

ShapesDemo::ShapesDemo()
{

// 	DirectX::XMFLOAT4X4 sSphereWorld[10];
// 	DirectX::XMFLOAT4X4 sCylWorld[10];
// 	DirectX::XMFLOAT4X4 sBoxWorld;
// 	DirectX::XMFLOAT4X4 sGridWorld;
// 	DirectX::XMFLOAT4X4 sCenterSphere;

	DirectX::XMMATRIX I = DirectX::XMMatrixIdentity();

	DirectX::XMStoreFloat4x4(&sGridWorld, I);
	DirectX::XMMATRIX boxScale = DirectX::XMMatrixScaling(2.0f, 1.0f, 2.0f);
	DirectX::XMMATRIX boxOffset = DirectX::XMMatrixTranslation(0.0f, 0.5f, 0.0f); //TODO z might be 4.0f
	DirectX::XMStoreFloat4x4(&sBoxWorld, DirectX::XMMatrixMultiply(boxScale, boxOffset));

	DirectX::XMMATRIX centerSphereScale = DirectX::XMMatrixScaling(2.0f, 2.0f, 2.0f);
	DirectX::XMMATRIX centerSphereOffset = DirectX::XMMatrixTranslation(0.0f, 2.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&sCenterSphere, DirectX::XMMatrixMultiply(centerSphereScale, centerSphereOffset));

	//5 rows of 2 cylinders and spheres per row
	for (int i = 0; i < 5; ++i)
	{
		DirectX::XMStoreFloat4x4(&sCylWorld[i * 2 + 0], //TODO WTF?????
			DirectX::XMMatrixTranslation(-5.0f, 1.5f, -10.0f + i * 5.0f));

		DirectX::XMStoreFloat4x4(&sCylWorld[i * 2 + 1],
			DirectX::XMMatrixTranslation(5.0f, 1.5f, -10.0f + i * 5.0f));

		DirectX::XMStoreFloat4x4(&sSphereWorld[i * 2 + 0],
			DirectX::XMMatrixTranslation(-5.0f, 3.5f, -10.0f + i * 5.0f));

		DirectX::XMStoreFloat4x4(&sSphereWorld[i * 2 + 1],
			DirectX::XMMatrixTranslation(5.0f, 3.5f, -10.0f + i * 5.0f));
	}

	SetSphereWorld(sSphereWorld);
	SetCylinderWorld(sCylWorld);
	SetBoxWorld(sBoxWorld);
	SetGridWorld(sGridWorld);
	SetCenterSphere(sCenterSphere);

// 	generator.CreateBox(1.0f, 1.0f, 1.0f, box);
// 	generator.CreateGrid(20.0f, 30.0f, 60, 40, grid);
// 	generator.CreateSphere(0.5f, 20, 20, sphere);
// 	generator.CreateCylinder(0.5f, 0.3f, 3.0f, 20, 20, cylinder);
// 
// 	//Cache the vertex offset to each object in the
// 	//concatenated vertex buffer
// 	mBoxVertexOffset = 0;
// 	mGridVertexOffset = UINT(box.vertices.size());
// 	mSphereVertexOffset = mGridVertexOffset + UINT(grid.vertices.size());
// 	mCylinderVertexOffset = mSphereVertexOffset + UINT(sphere.vertices.size());
// 
// 	//cache index count of each object
// 	mBoxIndexCount = UINT(box.indices.size());
// 	mGridIndexCount = UINT(grid.indices.size());
// 	mSphereIndexCount = UINT(sphere.indices.size());
// 	mCylinderIndexCount = UINT(cylinder.indices.size());
// 
// 	// Cache the starting index for each object in the concatenated
// 	// index buffer.
// 	mBoxIndexOffset = 0;
// 	mGridIndexOffset = UINT(mBoxIndexCount);
// 	mSphereIndexOffset = UINT(mGridIndexOffset + mGridIndexCount);
// 	mCylinderIndexOffset = UINT(mSphereIndexOffset + mSphereIndexCount);
// 
// 	UINT totalVertexCount =
// 		UINT(box.vertices.size()) +
// 		UINT(grid.vertices.size()) +
// 		UINT(sphere.vertices.size()) +
// 		UINT(cylinder.vertices.size());
// 	UINT totalIndexCount =
// 		mBoxIndexCount +
// 		mGridIndexCount +
// 		mSphereIndexCount +
// 		mCylinderIndexCount;
// 	// Extract the vertex elements we are interested in and pack the
// 	// vertices of all the meshes into one vertex buffer.
// 
// 	std::vector<Vertex_Demo> vertices(totalVertexCount);
// 
// 	DirectX::XMFLOAT4 black(0.0f, 0.0f, 0.0f, 1.0f);
// 	UINT k = 0;
// 
// 	for (size_t i = 0; i < box.vertices.size(); i++, k++)
// 	{
// 		vertices[k].pos = box.vertices[i].position;
// 		vertices[k].color = black;
// 	}
// 	for (size_t i = 0; i < grid.vertices.size(); i++, k++)
// 	{
// 		vertices[k].pos = grid.vertices[i].position;
// 		vertices[k].color = black;
// 	}
// 	for (size_t i = 0; i < sphere.vertices.size(); i++, k++)
// 	{
// 		vertices[k].pos = sphere.vertices[i].position;
// 		vertices[k].color = black;
// 	}
// 	for (size_t i = 0; i < cylinder.vertices.size(); i++, k++)
// 	{
// 		vertices[k].pos = cylinder.vertices[i].position;
// 		vertices[k].color = black;
// 	}
// 	AddBind(std::make_unique<VertexBuffer>(gfx, vertices, L"ShapesDemo"));
// 	std::vector<UINT> indices;
// 	indices.insert(indices.end(), box.indices.begin(), box.indices.end());
// 	indices.insert(indices.end(), grid.indices.begin(), grid.indices.end());
// 	indices.insert(indices.end(), sphere.indices.begin(), sphere.indices.end());
// 	indices.insert(indices.end(), cylinder.indices.begin(), cylinder.indices.end());
// 	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices, L"ShapesDemoIndexBuffer"));
// 
// 	AddBind(std::make_unique<VertexBuffer>(gfx, vertices, L"Shapes"));
// 	auto pVertexShader = std::make_unique<VertexShader>(gfx, L"CubeVS.cso");
// 	auto pVertexShaderBlob = pVertexShader->GetByteCode();
// 	AddBind(std::move(pVertexShader));
// 	const std::vector<D3D11_INPUT_ELEMENT_DESC> inputElemDesc =
// 	{
// 		{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
// 		D3D11_INPUT_PER_VERTEX_DATA, 0u},
// 		{"Color", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
// 		D3D11_INPUT_PER_VERTEX_DATA, 0u}
// 	};
// 	AddBind(std::make_unique<InputLayout>(gfx, pVertexShaderBlob, inputElemDesc, L"PositionAndColor"));
// 	AddBind(std::make_unique<PixelShader>(gfx, L"CubePS.cso"));
// 	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
// 	AddBind(std::make_unique<TransformConstantBuffer>(gfx, *this));


}

// DirectX::XMMATRIX ShapesDemo::Update(float dt, DirectX::XMMATRIX in_matrix) noexcept
// {
// 	return in_matrix;
// }

DirectX::XMMATRIX* ShapesDemo::GetSphereWorldArray() const noexcept
{
	return pm_SphereWorld;
}

//for pointer arithmetics
DirectX::XMMATRIX*& ShapesDemo::GetCylinderWorldArray() noexcept
{
	return pm_CylinderWorld;
}

DirectX::XMMATRIX ShapesDemo::Get_m_BoxWorld() const noexcept
{
	return m_BoxWorld;
}

DirectX::XMMATRIX ShapesDemo::Get_m_GridWorld() const noexcept
{
	return m_GridWorld;
}

DirectX::XMMATRIX ShapesDemo::Get_m_CenterSphere() const noexcept
{
	return m_CenterSphere;
}

void ShapesDemo::SetSphereWorld(DirectX::XMFLOAT4X4 source[10])
{
	for (size_t i = 0; i < 10; i++)
	{
		m_SphereWorld[i] = DirectX::XMLoadFloat4x4(&source[i]);
	}
	pm_SphereWorld = m_SphereWorld;
}

void ShapesDemo::SetCylinderWorld(DirectX::XMFLOAT4X4 source[10])
{
	for (size_t i = 0; i < 10; i++)
	{
		m_CylWorld[i] = DirectX::XMLoadFloat4x4(&source[i]);
	}
	pm_CylinderWorld = m_CylWorld;
}

void ShapesDemo::SetBoxWorld(DirectX::XMFLOAT4X4& source)
{
	m_BoxWorld = DirectX::XMLoadFloat4x4(&source);
}

void ShapesDemo::SetGridWorld(DirectX::XMFLOAT4X4& source)
{
	m_GridWorld = DirectX::XMLoadFloat4x4(&source);
}

void ShapesDemo::SetCenterSphere(DirectX::XMFLOAT4X4& source)
{
	m_CenterSphere = DirectX::XMLoadFloat4x4(&source);
}
