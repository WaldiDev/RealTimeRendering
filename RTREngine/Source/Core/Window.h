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

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* RWCreateWindow(const WindowDefinition& windowDefinition);
	};
}