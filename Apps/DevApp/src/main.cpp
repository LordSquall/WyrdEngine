/* engine headers */
#include <Osiris.h>

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
		PushLayer(new ExampleLayer());

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