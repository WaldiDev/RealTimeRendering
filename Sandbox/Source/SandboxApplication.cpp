
#include <RTREngine.h>

class ExampleLayer : public rtr::Layer
{

public:
	ExampleLayer()
		: Layer("Example")
	{}

	void OnAttach() override
	{
	}

	void OnDetach() override
	{
	}

	void OnUpdate() override
	{
	}

};

class Sandbox : public rtr::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
};

rtr::Application* rtr::CreateApplication()
{
	return new Sandbox();
}