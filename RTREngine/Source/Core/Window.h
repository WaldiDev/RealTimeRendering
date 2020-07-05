#pragma once

#include <string>

namespace rtr
{
	struct WindowDefinition
	{
		WindowDefinition()
			: Title(L"RTR Engine")
			, Width(1280u)
			, Height(720u)
		{}

		WindowDefinition(const std::wstring& title, unsigned int width, unsigned int height)
			: Title(title)
			, Width(width)
			, Height(height)
		{}

		std::wstring Title;
		unsigned int Width;
		unsigned int Height;
	};

	class Window
	{
	public:
		virtual ~Window() {}

		virtual void OnUpdate() = 0;
		virtual void OnResize() = 0;

		virtual unsigned int GetWindowWidth() const = 0;
		virtual unsigned int GetWindowHeight() const = 0;

		virtual unsigned int GetClientWidth() const = 0;
		virtual unsigned int GetClientHeight() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* RWCreateWindow(const WindowDefinition& windowDefinition);
	};
}