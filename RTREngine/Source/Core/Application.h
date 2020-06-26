#pragma once

#include "LayerStack.h"

namespace rtr
{
	class Layer;
	class Window;
	class Video;
	class ImguiLayer;

	class Application
	{
	public:
		Application();
		~Application();

		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		Window& GetWindow() { return *mWindow; }
		Video* GetVideo() { return mVideo; }

		static Application& Get() { return *sInstance; }

	private:
		Window* mWindow;

		Video* mVideo;

		ImguiLayer* mImguiLayer;

		LayerStack mLayerStack;

		bool mIsRunning;



		static Application* sInstance;
	};

	// Defined on client
	Application* CreateApplication();
}
