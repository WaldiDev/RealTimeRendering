

#include "Application.h"
#include "EntryPoint.h"

class Sandbox : public rtr::Application
{
public:
	Sandbox()
	{

	}
};

rtr::Application* rtr::CreateApplication()
{
	return new Sandbox();
}