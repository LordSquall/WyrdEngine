#include <Osiris.h>

#include "events/ApplicationEvent.h"
#include "events/KeyEvent.h"
#include "graphics/Shader.h"
#include "layers/ImGuiLayer.h"

#include "Resources.h"
#include "layers/LoadingLayer.h"
#include "layers/TestRenderLayer.h"

class ClientApplication : public Osiris::Application
{
public:
	ClientApplication()
	{
		/* Set the resources renderer context */
		m_Resources.SetRenderer(m_Renderer);

		PushOverlay(new Osiris::ImGuiLayer());
	}

	~ClientApplication()
	{

	}

private:
	Resources m_Resources;
};

Osiris::Application* Osiris::CreateApplication()
{
	return new ClientApplication();
}