#include <Osiris.h>

#include "events/ApplicationEvent.h"
#include "events/KeyEvent.h"

#include "EditorLayer.h"

class EditorApplication : public Osiris::Application
{
public:
	EditorApplication()
	{

	}

	~EditorApplication()
	{

	}
};

Osiris::Application* Osiris::CreateApplication()
{
	return new EditorApplication();
}