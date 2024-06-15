#pragma once
#include "Globals.h"
#include <d3d11.h>

class DXManager
{
	// global declarations
	IDXGISwapChain* swapchain;             // the pointer to the swap chain interface
	ID3D11Device* dev;                     // the pointer to our Direct3D device interface
	ID3D11DeviceContext* devcon;           // the pointer to our Direct3D device context

	ID3D11RenderTargetView* backBuffer;
	const float clearColor[4] = { 1.0f, 1.0f, 0.4f, 1.0f };

public :
	// function prototypes
	void InitD3D(HWND hWnd);     // sets up and initializes Direct3D
	void RenderFrame(void);
	void CleanD3D(void);         // closes Direct3D and releases memory

	//constructors
	DXManager();
};

