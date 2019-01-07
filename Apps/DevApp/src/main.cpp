#include <Osiris.h>

class ExampleLayer : public Osiris::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		OSR_CLIENT_INFO("ExampleLayer::Update");
	}

	void OnEvent(Osiris::Event& event) override
	{
		OSR_CLIENT_TRACE("{0}", event);
	}

};

class ClientApplication : public Osiris::Application
{
public:
	ClientApplication()
	{
		PushLayer(new ExampleLayer());
	}

	~ClientApplication()
	{

	}

};

Osiris::Application* Osiris::CreateApplication()
{
	return new ClientApplication();
}