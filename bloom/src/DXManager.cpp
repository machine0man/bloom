#include "DXManager.h"


/// <summary>
/// Sets vertexData and Draws
/// </summary>

void DXManager::SetVertexBuffer()
{
	UINT stride = sizeof(Vertex); //size of one element
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

	//set primitive topology
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	devcon->Draw(3, 0);
}

DXManager::DXManager() {}

HRESULT DXManager::LoadShader(const LPCWSTR filePath, ID3DBlob** S,  ESHADER_TYPE a_shaderType)
{
	ID3DBlob* pErrorBlob = nullptr;

	LPCSTR l_entryPoint= "";
	LPCSTR l_target= "";

	switch (a_shaderType)
	{
	case VERTEX:
	    l_entryPoint = "VSMain";
		l_target = "vs_4_0";
		break;
	case PIXEL:
		l_entryPoint = "PSMain";
		l_target = "ps_4_0";
		break;
	default: //shader undefined
		break;
	}

	HRESULT hr = D3DCompileFromFile(
		filePath, // pFileName
		nullptr,        // pDefines
		nullptr,        // pInclude
		l_entryPoint,       // pEntryPoint
		l_target,       // pTarget
		0,              // Flags1
		0,              // Flags2
		S,			// ppCode
		&pErrorBlob     // ppErrorMsgs
	);

	if (FAILED(hr)) {
		// Handle error
	}
	return hr;
}
//Shaders related
void DXManager::InitPipeline()
{
	//load shaders
	LoadShader(L"res/shaders/triangle.shader", &VS, ESHADER_TYPE::VERTEX);
	LoadShader(L"res/shaders/triangle.shader" , &PS , ESHADER_TYPE::PIXEL);

	// load and compile the two shaders
	//D3DX11CompileFromFile(L"shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
	//D3DX11CompileFromFile(L"shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);

	// encapsulate both shaders into shader objects
	dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

	//set the shader objects
	devcon->VSSetShader(pVS, 0, 0);
	devcon->PSSetShader(pPS,0,0);	
}

void DXManager::InitGraphics()
{
	CreateVertexBuffer();
	CopyDataToVertexBuffer();
}

void DXManager::CreateVertexBuffer()
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(Vertex) * 3;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	dev->CreateBuffer(&bd, NULL, &pVBuffer);       // create the buffer
}

void DXManager::CopyDataToVertexBuffer()
{
	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);   // map the buffer
	memcpy(ms.pData, vertices, sizeof(vertices));                // copy the data
	devcon->Unmap(pVBuffer, NULL);                                     // unmap the buffer
}

void DXManager::SetInputLayout()
{
	//layout description
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//setting data to input layout
	dev->CreateInputLayout(
		ied,
		2,
		VS->GetBufferPointer(),
		VS->GetBufferSize(),
		&pLayout
	);
	devcon->IASetInputLayout(pLayout);
}

// this function initializes and prepares Direct3D for use
void DXManager::InitD3D(HWND hWnd)
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferDesc.Width = SCREEN_WIDTH;                    // set the back buffer width
	scd.BufferDesc.Height = SCREEN_HEIGHT;                  // set the back buffer height
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = hWnd;                                // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // allow full-screen switching

	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&dev,
		NULL,
		&devcon);

	//RENDERING STUFF

	//get the address of the back buffer
	ID3D11Texture2D* pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	//use the backbuffer address to crete the render target
	dev->CreateRenderTargetView(pBackBuffer, NULL, &backBuffer);
	pBackBuffer->Release();

	//set the render target as the back buffer
	devcon->OMSetRenderTargets(1, &backBuffer, NULL);


	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;

	devcon->RSSetViewports(1, &viewport);


}

// this is the function used to render a single frame
void DXManager:: RenderFrame(void)
{
	// clear the back buffer to a deep blue
	devcon->ClearRenderTargetView(backBuffer, clearColor);

	// do 3D rendering on the back buffer here
	SetVertexBuffer();

	// switch the back buffer and the front buffer
	swapchain->Present(0, 0);
}
// this is the function that cleans up Direct3D and COM
void DXManager ::CleanD3D()
{
	swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

	// close and release all existing COM objects
	pVS->Release();
	pPS->Release();
	swapchain->Release();
	backBuffer->Release();
	dev->Release();
	devcon->Release();
}