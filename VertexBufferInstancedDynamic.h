#pragma once
#include "Bindable.h"

class VertexBufferInstancedDynamic : public Bindable
{
public:
	template <typename Vertices, typename Instanced>
	VertexBufferInstancedDynamic(Graphics& gfx, const std::vector<Vertices>& vertices,
		const std::vector<Instanced>& instancedData, const std::wstring& name = std::wstring())
	{ 
		stride[0] = sizeof(Vertices);
		stride[1] = sizeof(Instanced);
		//define it here, so Visual studio won't complain about header file stop
		//Immutable buffer for vertices
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufferDesc.ByteWidth = static_cast<UINT>(sizeof(Vertices) * vertices.size());//size of the structure
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0u;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = sizeof(Vertices); // size of the each element in the structure
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = vertices.data();
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		DX::ThrowIfFailed(GetDevice(gfx)->CreateBuffer(&bufferDesc, &initData, &pVertexBuffer));

		//dynamic buffer for Instanced data
		D3D11_BUFFER_DESC bufferDesc0;
		bufferDesc0.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc0.ByteWidth = static_cast<UINT>(sizeof(Instanced) * instancedData.size());//size of the structure
		bufferDesc0.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc0.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc0.MiscFlags = 0;
		bufferDesc0.StructureByteStride = 0u; // size of the each element in the structure
		DX::ThrowIfFailed(GetDevice(gfx)->CreateBuffer(&bufferDesc0, 0u, &pVertexDynamicInstanced));
		vBuffers[0] = pVertexBuffer;
		vBuffers[1] = pVertexDynamicInstanced;
#ifdef MY_DEBUG
		if (name != std::wstring())
		{
// 			gfx.SetDebugName(pVertexBuffer.Get(), name.c_str());
		}
#endif
	}

	void Bind(Graphics& gfx) noexcept override;
	ID3D11Buffer* Get_p_DynamicInstancedVertexBuffer() const noexcept;
private:
	UINT offset[2] {};
	UINT stride[2] {};
// 	Microsoft::WRL::ComPtr <ID3D11Buffer> pVertexDynamicInstanced;
// 	Microsoft::WRL::ComPtr <ID3D11Buffer> pVertexBuffer;
// 	Microsoft::WRL::ComPtr <ID3D11Buffer> vBuffers[2];
	ID3D11Buffer* pVertexDynamicInstanced;
 	ID3D11Buffer* pVertexBuffer;
 	ID3D11Buffer* vBuffers[2];



};