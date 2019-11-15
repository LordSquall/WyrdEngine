/* engine headers */
#include <Osiris.h>

#include "layers/Renderer2DLayer.h"
#include "core/EntryPoint.h"

/* local headers */
#include "layers/ExampleLayer.h"

#include "editor/layers/EditorLayer.h"
#include "editor/layers/EditorRenderer2DLayer.h"

class ClientApplication : public Osiris::Application
{
public:
	ClientApplication()
	{
		PushLayer(new Osiris::Editor::EditorRenderer2DLayer("Editor2DLayer"));
		PushOverlay(new Osiris::Editor::EditorLayer());
	}

	~ClientApplication()
	{

	}
};

Osiris::Application* Osiris::CreateApplication()
{
	return new ClientApplication();
}