/* engine headers */
#include <core/Application.h>

/* local headers */
#include "layers/ControlLayer.h"

#include "core/EntryPoint.h"

class PlayerApp : public Osiris::Application
{
public:
	PlayerApp(const AppProps& props) : Application(props)
	{
		if (PushLayer(new ControlLayer()) == false)
		{
			Close();
		}
	}

	~PlayerApp()
	{
		//system("pause");
	}
};

Osiris::Application* Osiris::CreateApplication()
{
	AppProps properties = AppProps();
	return new PlayerApp(properties);
}

void Osiris::OnPreAppCreation(Osiris::Application* application) { }