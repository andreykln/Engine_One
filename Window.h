#pragma once
#include "Keyboard.h"
#include "Graphics.h"
#include "Mouse.h"
#include "CustomException.h"
#include <sstream>
#include <optional>
#define ThrowWin32LastCustomException() Window::Win32Exception(__LINE__,__FILE__, GetLastError())
#define ThrowWin32CustomException(hr) Window::Win32Exception(__LINE__, __FILE__, hr)


class Window
{
public:
	class Win32Exception : public CustomException
	{
	public:
		Win32Exception(int in_line, const char* file, HRESULT hr);
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		std::string GetErrorString() const noexcept;
		HRESULT GetErrorCode() const noexcept;
		virtual const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
	private:
		HRESULT hr;
	};

private:

	 	class WindowClass
	{
	public:
		static const char* GetClassName();
		static HINSTANCE GetInstance();
	private:
		WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		~WindowClass();

		static constexpr const char* class_name = "Engine window";
		HINSTANCE hWindowInstance;
		static WindowClass wndClass;
	};
	
public:
	Window(const char* in_windowName, unsigned int in_width, unsigned int in_height);
	Window& operator=(const Window&) = delete;
	Window(const Window&) = delete;
	const char* GetWindowName();
	static std::optional<int> ProcessMessages();
	void SetTitle(const std::string& text);
	Graphics& GetGraphics();
	float GetPhi() const noexcept;
	float GetTheta() const noexcept;
	float GetRadius() const noexcept;
	~Window();
public:
	Keyboard kbd;
	Mouse mouse;
	float mPhi{0.25f * DirectX::XM_PI};
	float mTheta{1.5f * DirectX::XM_PI };
	float mRadius{10.0f};
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)noexcept;


	HWND handleWindow;
	Graphics* gfx = nullptr;
	const char* windowName = {};
	const int width;
	const int height;
	unsigned int wndPositionX = 1200;
	unsigned int wndPositionY = 900;


};

