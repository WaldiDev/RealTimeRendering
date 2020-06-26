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
		
		virtual unsigned int GetWidth() const override { return mDefinition.Width; }
		virtual unsigned int GetHeight() const override { return mDefinition.Height; }
		virtual void* GetNativeWindow() const override { return mHandle; }

	private:
		WindowDefinition mDefinition;
		
		HWND mHandle;
	};
}
