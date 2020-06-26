#include "Win32Window.h"
#include "Win32.h"

#include "Core/Logging/Log.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
namespace
{
	
	static LRESULT CALLBACK WinWindowCallback(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(windowHandle, message, wParam, lParam))
		{
			return 1;
		}

		LRESULT result = 0;
		switch (message)
		{
			case WM_CLOSE:
			case WM_DESTROY:
			{
				::PostQuitMessage(0);
			} break;
			default:
				result = ::DefWindowProc(windowHandle, message, wParam, lParam);
		}

		return result;
	}

	HWND Win32CreateWindow(const rtr::WindowDefinition& windowDefinition)
	{
		RTR_ENGINE_TRACE("Start Win32CreateWindow");

		const DWORD windowStyle = WS_OVERLAPPEDWINDOW;

		// AdjustWindowRect computes based on the given windowStyle the size of the window border, when we create the window we have to add this values, otherwise the created window will be too small.
		RECT windowBorderRect = {};
		::AdjustWindowRectEx(&windowBorderRect, windowStyle, FALSE, 0);

		HMODULE instance = ::GetModuleHandle(nullptr);

		WNDCLASSEX windowClass		= {};
		windowClass.style			= CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc		= WinWindowCallback;
		windowClass.hInstance		= instance;
		windowClass.hbrBackground	= static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
		windowClass.lpszClassName	= windowDefinition.Title.c_str();
		windowClass.hCursor			= LoadCursor(nullptr, IDC_ARROW);
		windowClass.cbSize			= sizeof(WNDCLASSEX);

		if (::RegisterClassEx(&windowClass) == 0)
		{
			RTR_ENGINE_ERROR("Error in Win32CreateWindow: RegisterClassEx failed");
			return nullptr;
		}

		HWND handle = ::CreateWindowEx(
			0,
			windowClass.lpszClassName,
			windowDefinition.Title.c_str(),
			windowStyle,
			0, 0,
			windowDefinition.Width + (windowBorderRect.right - windowBorderRect.left),
			windowDefinition.Height + (windowBorderRect.bottom - windowBorderRect.top),
			nullptr, nullptr, instance, nullptr);

		::ShowWindow(handle, SW_SHOWDEFAULT);

		RTR_ENGINE_TRACE("Finished Win32CreateWindow");
		return handle;
	}
}

namespace rtr
{
	Win32Window::Win32Window(const WindowDefinition& windowDefinition)
		: mDefinition(windowDefinition)
		, mHandle(::Win32CreateWindow(windowDefinition))
	{
		// Todo (Marcus): Assert if handle == nullptr
	}

	Win32Window::~Win32Window()
	{
		::DestroyWindow(mHandle);

		HMODULE instance = ::GetModuleHandle(nullptr);
		::UnregisterClass(mDefinition.Title.c_str(), instance);
	}

	void Win32Window::OnUpdate()
	{
		MSG message = {};
		while (::PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
		{
			switch (message.message)
			{
			case WM_QUIT:
			{
			} break;
			default:
				break;
			}

			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
	}

	Window* Window::RWCreateWindow(const WindowDefinition& windowDefinition)
	{
		return new Win32Window(windowDefinition);
	}
}