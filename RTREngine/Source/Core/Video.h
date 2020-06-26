#pragma once

namespace rtr
{
	class Window;

	class Video
	{
	public:
		virtual ~Video() {}

		virtual void BeginRender() = 0;
		virtual void EndRender() = 0;

		static Video* CreateVideo(const Window& window);
	};
}