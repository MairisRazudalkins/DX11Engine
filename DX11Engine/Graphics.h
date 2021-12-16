#pragma once
#include "GraphicsCore.h"

class Controller;
class Orbit;
class PostProcessorShader;
class Plane;
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

	PostProcessorShader*	postProcShader;
	ID3D11Texture2D*		backBuffer;

	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

	void CreateInputLayouts(ID3DBlob* vsBlob, ID3DBlob* psBlob);
	void CreateDepthStencilView();
	void CreateDepthStencilBuff();
	void CreateConstBuffers();
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

	void SelectObj();

	Mesh* mesh;
	SkyDome* skyDome;
	Plane* plane;
	Orbit* orbit;

	Controller* controller;

	//ModelConstBuffer cb;
};