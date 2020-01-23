#include "WindowsEdited.h"
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <sstream>
#include <string>

#include "Window.h"
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

	Window wnd("window", 800, 600);
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