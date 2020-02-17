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

	return -1;
}
