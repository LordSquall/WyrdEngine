#include <Osiris.h>

#include "events/ApplicationEvent.h"
#include "events/KeyEvent.h"
#include "graphics/Shader.h"

#include "layers/imgui/ImGuiLayer.h"
#include "layers/LoadingLayer.h"

class ClientApplication : public Osiris::Application
{
public:
	ClientApplication()
	{
		PushLayer(new LoadingLayer());
		PushOverlay(new Osiris::ImGuiLayer());
	}

	~ClientApplication()
	{

	}
};

Osiris::Application* Osiris::CreateApplication()
{
	return new ClientApplication();
}