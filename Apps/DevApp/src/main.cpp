/* engine headers */
#include <Osiris.h>

#include "layers/Renderer2DLayer.h"
#include "core/EntryPoint.h"

/* local headers */
#include "layers/ExampleLayer.h"

/* if editor is enabled we want to include the inbuild editor layers */
#ifdef OSR_EDITOR_ENABLED 
#include "editor/layers/EditorLayer.h"
#endif

class ClientApplication : public Osiris::Application
{
public:
	ClientApplication()
	{
		/*PushLayer(new ExampleLayer("Sprite Layer 1", 0));
		PushLayer(new ExampleLayer("Sprite Layer 2", 1));
		PushLayer(new ExampleLayer("Sprite Layer 3", 2));
		PushLayer(new ExampleLayer("Sprite Layer 4", 3));*/

		PushLayer(new Osiris::Layers::Renderer2DLayer("2D Rendering Layer"));

#ifdef OSR_EDITOR_ENABLED
		PushOverlay(new Osiris::Editor::EditorLayer());
#endif
		
	}

	~ClientApplication()
	{

	}
};

Osiris::Application* Osiris::CreateApplication()
{
	return new ClientApplication();
}