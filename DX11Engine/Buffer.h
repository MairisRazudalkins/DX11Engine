#pragma once

#ifndef BUFFER
	#define BUFFER

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

public:
	~Buffer() { if (buffer) buffer->Release(); }

	ID3D11Buffer* GetBuffer() { return buffer; }
	UINT GetBufferSize() { return bufferSize; }

	template<class T>
	static Buffer* CreateConstBuffer() 
	{
		Buffer* buffer = new Buffer(0);

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(T);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;

		if (FAILED(Graphics::GetDevice()->CreateBuffer(&bd, nullptr, &buffer->buffer)))
			Quit(Logger::Fatal, "Failed to create constant buffer");

		return buffer;
	}

	template<class T>
	static Buffer* CreateBuffer(T* data, UINT size, BufferBindFlag bindFlag)
	{
		Buffer* buffer = new Buffer(size);

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(T) * size;
		bufferDesc.BindFlags = bindFlag;
		bufferDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA bufferData;
		ZeroMemory(&bufferData, sizeof(bufferData));
		bufferData.pSysMem = data;

		if (FAILED(Graphics::GetDevice()->CreateBuffer(&bufferDesc, &bufferData, &buffer->buffer)))
		{
			delete buffer;
			return nullptr;
		}

		return buffer;
	}
};

#endif