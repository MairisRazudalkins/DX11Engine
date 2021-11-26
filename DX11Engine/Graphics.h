#pragma once
#include "GraphicsCore.h"

class Mesh;
class SkyDome;

using namespace ShaderBuffers;

class Graphics
{
	friend class EngineLoop;

	Graphics() = default;
	~Graphics();

	static Graphics*		inst;

	ID3D11Device*			device;
	ID3D11DeviceContext*	deviceContext;

	IDXGISwapChain*			swapChain;

	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D*		depthStencilBuffer;

	ID3D11InputLayout*		pixelInputLayout;
	ID3D11InputLayout*		vertexInputLayout;

	D3D_DRIVER_TYPE         driverType;
	D3D_FEATURE_LEVEL       featureLevel;

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

	void CreateInputLayouts(ID3DBlob* vsBlob, ID3DBlob* psBlob);
	void CreateDepthStencilView();
	void CreateDepthStencilBuff();
	void CreateConstBuffers();
	void InitializeShaders();
	void CreateSwapChain();
	void CreateViewport();

	void Render();

	static Graphics* GetInst() { return inst == nullptr ? inst = new Graphics() : inst; }
	void Initialize(int nCmdShow);

	int winHeight = 500, winWidth = 500;

public:
	static ID3D11Device* GetDevice() { return inst->device; }
	static ID3D11DeviceContext* GetDeviceContext() { return inst->deviceContext; }
	static D3D_DRIVER_TYPE GetDriverType() { return inst->driverType; }

	static int GetWindowHeight() { return GetInst()->winHeight; }
	static int GetWindowWidth() { return GetInst()->winWidth; }

	// DEBUG
	DirectX::XMFLOAT4X4	world;
	DirectX::XMFLOAT4X4	_view;
	DirectX::XMFLOAT4X4	_projection;

	Buffer* modelConstBuffer;
	Buffer* lightConstBuffer;
	Buffer* mtrlConstBuffer;

	Mesh* mesh;
	SkyDome* skyDome;

	ModelConstBuffer modelCB;
	LightingConstBuffer lightCB;
	MaterialConstBuffer mtrlCB;

	static ModelConstBuffer GetModelCB() { return inst->modelCB; }

	//ModelConstBuffer cb;
};