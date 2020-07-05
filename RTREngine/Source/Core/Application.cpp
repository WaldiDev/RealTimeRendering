#include "Application.h"
#include "Layer.h"
#include "Window.h"
#include "Video.h"
#include "Imgui/ImguiLayer.h"

namespace rtr
{
	Application* Application::sInstance = nullptr;

	Application::Application()
		: mWindow(nullptr)
		, mVideo(nullptr)
		, mImguiLayer(nullptr)
		, mLayerStack()
		, mIsRunning(false)
	{
		// Todo (Marcus): Assert if instance != nullptr
		sInstance = this;
		
		mWindow = Window::RWCreateWindow(WindowDefinition());
		mVideo = Video::CreateVideo(GetWindow());

		mImguiLayer = new ImguiLayer();
		PushOverlay(mImguiLayer);
	}

	Application::~Application()
	{
		delete mVideo;
		delete mWindow;
	}

	void Application::Run()
	{
		mIsRunning = true;

		while (mIsRunning)
		{
			mWindow->OnUpdate();

			for (Layer* layer : mLayerStack)
			{
				layer->OnUpdate();
			}

			mImguiLayer->Begin();
			mVideo->BeginRender();

			mImguiLayer->End();
			mVideo->EndRender();
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		mLayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		mLayerStack.PushOverlay(layer);
	}

	void Application::Resize(uint32_t width, uint32_t height)
	{
		if (mVideo && width > 0u && height > 0u)
		{
			mWindow->OnResize();
			mVideo->OnResize(width, height);
		}
	}

}
