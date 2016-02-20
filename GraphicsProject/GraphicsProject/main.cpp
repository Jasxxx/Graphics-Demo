// Author: Jason Boza

// Reference http://www.rastertek.com/dx11tut10.html
// Reference http://www.braynzarsoft.net/index.php?p=D3D11SIMPLELIGHT#still
// Reference http://www.braynzarsoft.net/index.php?p=D3D11CUBEMAP
// Reference http://en.wikipedia.org/wiki/Sine_wave

//************************************************************
//************ INCLUDES & DEFINES ****************************
//************************************************************
#pragma once

#include <ctime>
#include "XTime.h"
#include "Renderer.h"
#include "GraphicsStructs.h"
#include <vld.h>

using namespace DirectX;
using namespace std;

#define BACKBUFFER_WIDTH	1280	
#define BACKBUFFER_HEIGHT	720

//************************************************************
//************ SIMPLE WINDOWS APP CLASS **********************
//************************************************************

class MAIN_APP
{
	HINSTANCE						application;
	WNDPROC							appWndProc;
	HWND							window;

	// Declaration of Time object for time related use
	XTime Time;

	// Camera Declarations
	XMMATRIX Transform;
	float RotateX = 0.0f;
	float RotateY = 0.0f;

public:
	MAIN_APP(HINSTANCE hinst, WNDPROC proc);
	bool Run();
	bool ShutDown();
};

//************************************************************
//************ CREATION OF OBJECTS & RESOURCES ***************
//************************************************************

MAIN_APP::MAIN_APP(HINSTANCE hinst, WNDPROC proc)
{
	application = hinst;
	appWndProc = proc;
	WNDCLASSEX  wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = appWndProc;
	wndClass.lpszClassName = L"DirectXApplication";
	wndClass.hInstance = application;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME);
	RegisterClassEx(&wndClass);

	RECT window_size = { 0, 0, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT };
	AdjustWindowRect(&window_size, WS_OVERLAPPEDWINDOW, false);

	window = CreateWindow(L"DirectXApplication", L"The Worst", WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX),
		CW_USEDEFAULT, CW_USEDEFAULT, window_size.right - window_size.left, window_size.bottom - window_size.top,
		NULL, NULL, application, this);

	ShowWindow(window, SW_SHOW);

	Renderer::Initialize(window, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT);
	Renderer::ResizeBuffers();
}

//************************************************************
//************ EXECUTION *************************************
//************************************************************

bool MAIN_APP::Run()
{
	Time.Signal();

	Renderer::Render();
	return true;
}

//************************************************************
//************ DESTRUCTION ***********************************
//************************************************************

bool MAIN_APP::ShutDown()
{
	Renderer::Shutdown();
	UnregisterClass(L"DirectXApplication", application);
	return true;
}

//************************************************************
//************ WINDOWS RELATED *******************************
//************************************************************

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	srand(unsigned int(time(0)));
	MAIN_APP myApp(hInstance, (WNDPROC)WndProc);
	MSG msg; ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT && myApp.Run())
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	myApp.ShutDown();
	return 0;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (GetAsyncKeyState(VK_ESCAPE))
		message = WM_DESTROY;
	switch (message)
	{
	case (WM_DESTROY) : { PostQuitMessage(0); }
						break;
	case(WM_RBUTTONDOWN) :
	{

	}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
