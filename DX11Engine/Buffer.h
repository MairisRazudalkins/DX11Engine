#pragma once
#include "CoreMinimal.h"
#include "Graphics.h"

enum BufferBindFlag
{
	Vertex = D3D11_BIND_VERTEX_BUFFER,
	Index = D3D11_BIND_INDEX_BUFFER
};

class Buffer
{
	ID3D11Buffer* buffer;
	UINT bufferSize = 0;

	Buffer(UINT size) : bufferSize(size) {};
	Buffer(const Buffer& rhs) = default;

public:
	~Buffer() { if (buffer) buffer->Release(); }

	ID3D11Buffer* GetBuffer() { return buffer; }
	UINT GetBufferSize() { return bufferSize; }

	template<class T>
	static Buffer* CreateBuffer(T* data, UINT size, BufferBindFlag bindFlag)
	{
		Buffer* newBuffer = new Buffer(size);

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(T) * size;
		bufferDesc.BindFlags = bindFlag;
		bufferDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA bufferData;
		ZeroMemory(&bufferData, sizeof(bufferData));
		bufferData.pSysMem = data;

		if (FAILED(Graphics::GetDevice()->CreateBuffer(&bufferDesc, &bufferData, &newBuffer->buffer)))
		{
			delete newBuffer;
			return nullptr;
		}

		return newBuffer;
	}
};