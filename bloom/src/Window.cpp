#include "Window.h"

HWND Window::GetRef()
{
	return hWnd;
}
Window::Window(HINSTANCE hInstance, int nCmdShow)
{
	this->nCmdShow = nCmdShow;
	this->hInstance = hInstance;

	//LogMessage(TEXT("Creating Window!"));
}

Window::~Window() {}
void Window::ViewWindow()
{
	//show Window
	ShowWindow(hWnd, nCmdShow);
}
void Window::MakeWindow(WNDPROC winproc)
{
	//clearing memory
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	//set data to class instance
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = winproc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass1";

	//register window class
	RegisterClassEx(&wc);

	//sizing the window
	RECT wr = { 0,0,500,400 }; //size,postion
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	//create window and set data
	hWnd = CreateWindowEx(NULL,
		L"WindowClass1",    // name of the window class
		L"Our First Windowed Program",   // title of the window
		WS_OVERLAPPEDWINDOW,    // window style
		0,    // x-position of the window
		0,    // y-position of the window
		SCREEN_WIDTH,    // width of the window
		SCREEN_HEIGHT,    // height of the window
		NULL,    // we have no parent window, NULL
		NULL,    // we aren't using menus, NULL
		hInstance,    // application handle
		NULL);    // used with multiple windows, NULL
}