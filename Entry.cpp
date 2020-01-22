#include "WindowsEdited.h"
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <sstream>
#include <string>
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
HINSTANCE hInst;
std::ostringstream oss;
const char* title = "Esc pressed";
std::string input_string;

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow)
{
	const char* class_name = "class name";

	RECT rectangle;
	rectangle.left = 0;
	rectangle.right = 800 ;
	rectangle.top = 0;
	rectangle.bottom = 600 ;


	AdjustWindowRect(&rectangle, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);


	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = class_name;
	wcex.hIconSm = NULL;
	RegisterClassEx(&wcex);

	const char* window_name = "window name";
	hInst = hInstance;
	HWND hWnd = CreateWindowEx(0,
		class_name,
		window_name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		600, 300,
		rectangle.right - rectangle.left, rectangle.bottom - rectangle.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hWnd, SW_SHOW);

	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.lParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_MOUSEMOVE:
	{
		POINTS points = MAKEPOINTS(lParam);
		std::stringstream oss;
		oss << "x: " << points.x << " Y: " << points.y;
		SetWindowTextA(hWnd, oss.str().c_str());
		break;
	}
	case WM_CHAR:
	{
		switch (wParam)
		{
		case 0x1B:
			SetWindowTextA(hWnd, title);
			oss.str("");
			break;
		default:
		{
			oss << unsigned char(wParam);
			input_string = oss.str();
			SetWindowTextA(hWnd, input_string.c_str());
			break;
		}
		}
	}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
	
}


/*
	switch (message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_MOUSEMOVE:
		{
			POINTS points = MAKEPOINTS(lParam);
			std::stringstream oss;
			oss << "x: " << points.x << " Y: " << points.y;
			SetWindowTextA(hWnd, oss.str().c_str());
			break;
		}
		case WM_CHAR:
		{
			switch (wParam)
			{
			case 0x1B:
				SetWindowTextA(hWnd, title);
				oss.str("");
				break;
			default:
			{
				oss << unsigned char(wParam);
				input_string = oss.str();
				SetWindowTextA(hWnd, input_string.c_str());
				break;
			}
			}
		}

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}
		return 0;


*/