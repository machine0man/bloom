#pragma once
#include "Globals.h"
#include "d3d11.h"
#include "d3dcompiler.h"
#include <DirectXMath.h>

using namespace DirectX;

class DXManager
{
public:
	// function prototypes
	void InitD3D(HWND hWnd);     // sets up and initializes Direct3D
	void RenderFrame(void);
	void CleanD3D(void);         // closes Direct3D and releases memory

	//shaders related
	void InitPipeline();
	void SetInputLayout();
	void InitGraphics();
	void SetVertexBuffer();

	//constructors
	DXManager();

private:

	HRESULT LoadShader(const LPCWSTR filePath, ID3DBlob** S, ESHADER_TYPE a_shaderType);
	void CreateVertexBuffer();
	void CopyDataToVertexBuffer();

	// global declarations
	IDXGISwapChain* swapchain;             // the pointer to the swap chain interface
	ID3D11Device* dev;                     // the pointer to our Direct3D device interface
	ID3D11DeviceContext* devcon;           // the pointer to our Direct3D device context

	ID3D11RenderTargetView* backBuffer;
	const float clearColor[4] = { 0.0f, 0.0f, 0.f, 1.0f };

	//shader related vars
	ID3D11VertexShader* pVS;    // the vertex shader
	ID3D11PixelShader* pPS;     // the pixel shader
	ID3DBlob* VS, * PS;
	ID3D11Buffer* pVBuffer;
	ID3D11InputLayout* pLayout;

	// Define vertex data
	Vertex vertices[3] =
	{
		{ XMFLOAT3(0.0, 0.5, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.45f, -0.5f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-0.45f, -0.5f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }
	};
};