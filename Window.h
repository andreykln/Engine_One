#pragma once
#include "WindowsEdited.h"
#include <string>
#include <sstream>
class Window
{
	class WindowClass
	{
	public:
		WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static const char* GetClassName();
		static HINSTANCE GetInstance();
	private:
		static constexpr const char* class_name = "Engine window";
		HINSTANCE hWindowInstance;
		static WindowClass wndClass;
	public:
		~WindowClass();
	};

public:
	Window(const char* in_windowName, unsigned int in_width, unsigned int in_height);
	Window& operator=(const Window&) = delete;
	Window(const Window&) = delete;
	const char* GetWindowName();
	static int ProcessMessages();
	void SetTitle(const std::string& text);
	~Window();
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	HWND handleWindow;
	const char* windowName = {};
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int wndPositionX = 600;
	unsigned int wndPositionY = 300;


};

