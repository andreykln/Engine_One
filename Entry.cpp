#include "WindowsEdited.h"
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <sstream>
#include <string>

#include "App.h"
#include "Window.h"

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow)
{

	try
	{
		App::App().Go();

	}
	catch (const CustomException& e)
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception & e)
	{
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);

	}
 	//App::App().Go();

	return -1;
}

// LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
// {
// 	switch (message)
// 	{
// 	case WM_CLOSE:
// 		PostQuitMessage(0);
// 		break;
// 
// 	case WM_MOUSEMOVE:
// 	{
// 		POINTS points = MAKEPOINTS(lParam);
// 		std::stringstream oss;
// 		oss << "x: " << points.x << " Y: " << points.y;
// 		SetWindowTextA(hWnd, oss.str().c_str());
// 		break;
// 	}
// 	case WM_CHAR:
// 	{
// 		switch (wParam)
// 		{
// 		case 0x1B:
// 			SetWindowTextA(hWnd, title);
// 			oss.str("");
// 			break;
// 		default:
		//{
		//	oss << unsigned char(wParam);
// 			input_string = oss.str();
// 			SetWindowTextA(hWnd, input_string.c_str());
// 			break;
// 		}
// 		}
// 	}
// 
// 	default:
// 		return DefWindowProc(hWnd, message, wParam, lParam);
// 		break;
// 	}
// 	return 0;
// 	
// }
// 
// 
