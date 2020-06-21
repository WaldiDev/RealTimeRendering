#pragma once

namespace rtr
{
	class Application
	{
	public:
		Application();
		
		void Run();
	};

	// Defined on client
	Application* CreateApplication();
}
