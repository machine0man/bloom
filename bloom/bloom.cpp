#include "ImguiTest.h"
#include "windows.h"
#include <iostream>

#include "window.h"
#include "DXManager.h"

#pragma region Function Declarations
int WINAPI WinMain(HINSTANCE a_hInstance,
	HINSTANCE a_hPrevInstance,
	LPSTR a_lpCmdLine,
	int a_nCmdShow
);
void CreateMsgBox();
int CreateImguiTest();

int WINAPI MakeWindow(HINSTANCE a_hInstance,
	HINSTANCE a_hPrevInstance,
	LPSTR a_lpCmdLine,
	int a_nCmdShow);

//main message handler function
LRESULT CALLBACK WindowProc(HWND a_hWnd,
	UINT a_message,
	WPARAM a_wParam,
	LPARAM a_lParam
);
void LogMessage(LPCWSTR a_msg);
void LogMessage(LPCWSTR a_msg)
{
	MessageBox(NULL,
		a_msg,
		L"Message",
		MB_OK);
}

#pragma endregion




int WINAPI WinMain(HINSTANCE a_hInstance,
	HINSTANCE a_hPrevInstance,
	LPSTR a_lpCmdLine,
	int a_nCmdShow)
{
	//CreateImguiTest();
	//CreateMsgBox();
	return MakeWindow(a_hInstance,
		a_hPrevInstance,
		a_lpCmdLine,
		a_nCmdShow);

	std::getchar();

	return 0;
}

void CreateMsgBox()
{
	MessageBox(NULL,
		L"Hello World!",
		L"Just another Hello World program!",
		MB_ICONEXCLAMATION | MB_RETRYCANCEL);
}

int CreateImguiTest()
{
	ImguiTest l_test;
	return l_test.Run();
}

std::unique_ptr<DXManager> l_dxManager;

int WINAPI MakeWindow(HINSTANCE a_hInstance,
	HINSTANCE a_hPrevInstance,
	LPSTR a_lpCmdLine,
	int a_nCmdShow)
{

	std::unique_ptr<Window> l_window = std::make_unique<Window>(a_hInstance, a_nCmdShow);
	l_dxManager = std::make_unique<DXManager>();


	l_window->MakeWindow(WindowProc);
	l_window->ViewWindow();

	l_dxManager->InitD3D(l_window->GetRef());

	//shaders/drawing related
	l_dxManager->InitPipeline();
	l_dxManager->SetInputLayout();
	l_dxManager->InitGraphics();

	MSG l_msg;

	while (true) //game loop
	{
		if (PeekMessage(&l_msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&l_msg);
			DispatchMessage(&l_msg);
		}
		else
		{
			//handle the other game events and render frame
			l_dxManager->RenderFrame();
		}
	}

	l_dxManager->CleanD3D();

	return l_msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND a_hWnd, UINT a_message, WPARAM a_wParam, LPARAM a_lParam)
{
	switch (a_message)
	{
	case WM_DESTROY:
	{
		//close the application
		//LogMessage(TEXT("close botn was clicked"));
		PostQuitMessage(0);
		return 0;
	}
	case WM_QUIT: //quit
	{
		return 0;
	}

	default:
		return DefWindowProc(a_hWnd, a_message, a_wParam, a_lParam);
		break;
	}
}