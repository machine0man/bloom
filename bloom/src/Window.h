#pragma once
#include "Globals.h"
#include "windows.h"

class Window
{
private:
	HWND hWnd;
	WNDCLASSEX wc;
	HINSTANCE hInstance;
	int nCmdShow;

public:
	HWND GetRef();
	Window(HINSTANCE hInstance, int nCmdShow);
	~Window();
	void ViewWindow();
	void MakeWindow(WNDPROC winproc);
};