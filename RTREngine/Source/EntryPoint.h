#pragma once

#include <iostream>

#ifdef RTR_PLATFORM_WINDOWS

extern rtr::Application* rtr::CreateApplication();

int main(int argc, char** argv)
{
	rtr::Application* app = rtr::CreateApplication();
	app->Run();

	delete app;
}

#endif //RTR_PLATFORM_WINDOWS