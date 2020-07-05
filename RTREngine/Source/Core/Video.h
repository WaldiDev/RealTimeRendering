#pragma once
#include "stdint.h"

namespace rtr
{
	class Window;

	class Video
	{
	public:
		virtual ~Video() {}

		virtual void OnResize(uint32_t width, uint32_t height) = 0;

		virtual void BeginRender() = 0;
		virtual void EndRender() = 0;

		static Video* CreateVideo(const Window& window);
	};
}