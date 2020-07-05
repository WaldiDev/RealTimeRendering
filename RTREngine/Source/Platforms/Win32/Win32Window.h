#pragma once

#include "Core/Window.h"

struct HWND__;
typedef HWND__* HWND;

namespace rtr
{
	class Win32Window : public Window
	{
	public:
		explicit Win32Window(const WindowDefinition& windowDefinition);
		virtual ~Win32Window();
	
		virtual void OnUpdate() override;
		
		virtual void OnResize() override;

		virtual unsigned int GetWindowWidth() const override { return mDefinition.Width; }
		virtual unsigned int GetWindowHeight() const override { return mDefinition.Height; }
		
		virtual unsigned int GetClientWidth() const override { return mClientWidth; }
		virtual unsigned int GetClientHeight() const override { return mClientHeight; }

		virtual void* GetNativeWindow() const override { return mHandle; }

	private:
		WindowDefinition mDefinition;
		
		HWND mHandle;

		unsigned int mClientWidth;
		unsigned int mClientHeight;
	};
}
