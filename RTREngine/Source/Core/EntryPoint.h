#pragma once

#include "Logging/Log.h"

#ifdef RTR_PLATFORM_WINDOWS

extern rtr::Application* rtr::CreateApplication();

int main(int argc, char** argv)
{
	rtr::Log::Init();
	RTR_ENGINE_INFO("Initialize Logging");

	rtr::Application* app = rtr::CreateApplication();
	app->Run();

	delete app;
}

#endif //RTR_PLATFORM_WINDOWS