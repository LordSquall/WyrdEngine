/* engine headers */
#include <Osiris.h>

/* local headers */
#include "layers/ExampleLayer.h"

#include "core/EntryPoint.h"

class WindowCreationApp : public Osiris::Application
{
public:
	WindowCreationApp(const AppProps& props) : Application(props)
	{
		if (PushLayer(new ExampleLayer()) == false)
		{
			Close();
		}
	}

	~WindowCreationApp()
	{
	}
};

Osiris::Application* Osiris::CreateApplication()
{
	AppProps properties = AppProps();

	properties.windowProps.Title = "01 - Window Creation";

	return new WindowCreationApp(properties);  
}