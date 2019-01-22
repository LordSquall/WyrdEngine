#include <Osiris.h>

#include "events/ApplicationEvent.h"
#include "events/KeyEvent.h"
#include "graphics/Shader.h"

class ClientApplication : public Osiris::Application
{
public:
	ClientApplication()
	{
		PushOverlay(new Osiris::ImGuiLayer());
		PushOverlay(new LoadingLayer());
	}

	~ClientApplication()
	{

	}
};

Osiris::Application* Osiris::CreateApplication()
{
	return new ClientApplication();
}