#include "Window.h"

Window::WindowClass Window::WindowClass::wndClass;
// ======  WINDOW CLASS
Window::WindowClass::WindowClass()
	: hWindowInstance(GetModuleHandle(nullptr))
{
	WNDCLASSEX windowclass = {};
	windowclass.cbSize = sizeof(windowclass);
	windowclass.style = CS_OWNDC | CS_VREDRAW;
	windowclass.lpfnWndProc = HandleMsgSetup;
	windowclass.cbClsExtra = 0u;
	windowclass.cbWndExtra = 0u;
	windowclass.hInstance = GetInstance();
	windowclass.hIcon = NULL;
	windowclass.hCursor = NULL;
	windowclass.hbrBackground = NULL;
	windowclass.lpszMenuName = NULL;
	windowclass.lpszClassName = GetClassName();
	windowclass.hIconSm = NULL;
	RegisterClassEx(&windowclass);
}

const char* Window::WindowClass::GetClassName()
{
	return class_name;
}

HINSTANCE Window::WindowClass::GetInstance()
{
	return wndClass.hWindowInstance;
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(class_name, GetInstance());

}
// ======  WINDOW CLASS


Window::Window(const char* in_windowName, unsigned int in_width, unsigned int in_height)
	: windowName(in_windowName), width(in_width), height(in_height)
{
	RECT rectangle;
	rectangle.left = 0;
	rectangle.right = width;
	rectangle.top = 0;
	rectangle.bottom = height;
	AdjustWindowRect(&rectangle, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	handleWindow = CreateWindowEx(
	0u, WindowClass::GetClassName(),
		GetWindowName(),
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		wndPositionX, wndPositionY,
		rectangle.right - rectangle.left, rectangle.bottom - rectangle.top,
		nullptr, nullptr,
		WindowClass::GetInstance(),
		this); // that was nullptr and error
	ShowWindow(handleWindow, SW_SHOW);
}

const char* Window::GetWindowName()
{
	return windowName;
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg;
	while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return (int)msg.wParam;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return {};
}

void Window::SetTitle(const std::string& text)
{
	SetWindowTextA(handleWindow, text.c_str());
}

Window::~Window()
{
	DestroyWindow(handleWindow);
}

//create pointer to instance of the window into win API, so they will be handled by custom function
LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	//lParam and wPara,- message parameters, long ptr and unsigned int ptr respectively
	if (msg == WM_NCCREATE) // when window is first created, it receives this message via WindowProc
	{
		//extract ptr to window class from window creation, CREATESTRUCT is essentially is the same as CreateWindow()
		//but it also takes LPVOID lpCreateParams, which is custom parameter which we pass from CreateWindow (...., this)
		const CREATESTRUCT* const pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams); //and turn it into c++ class 

		//this function lest set data stored at Windows API side
		//GWLP_USERDATA lets store user defined data, for which we need to store pointer to c++ class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// GWLP_WNDPROC - to set custom windows procedure
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		//return created API pointer to custom message handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	//if something wrong, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//ptr to window class
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);

}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	const char* test_text = "hello";
	switch (msg)
	{

	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000) || kbd.IsAutoRepeatEnabled())
		{
			kbd.OnKeyPressed(unsigned char(wParam));
			SetWindowTextA(hWnd, test_text);
		}
		break;



// 	case WM_MOUSEMOVE:
// 	{
// 		POINTS points = MAKEPOINTS(lParam);
// 		std::stringstream oss;
// 		oss << "x: " << points.x << " Y: " << points.y;
// 		SetWindowTextA(hWnd, oss.str().c_str());
// 		break;
// 	}
		
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
