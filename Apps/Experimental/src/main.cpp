/* engine headers */
#include <Osiris.h>

/* local headers */
#include "layers/PlaygroundLayer.h"

#include "core/EntryPoint.h"

class ExperimentialApp : public Osiris::Application
{
public:
	ExperimentialApp(const AppProps& props) : Application(props)
	{
		if (PushLayer(new PlaygroundLayer()) == false)
		{
			Close();
		}
	}

	~ExperimentialApp()
	{
	}
};

Osiris::Application* Osiris::CreateApplication()
{
	AppProps properties = AppProps();
	return new ExperimentialApp(properties);
}