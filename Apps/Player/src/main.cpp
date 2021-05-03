/* engine headers */
#include <core/Application.h>

/* local headers */
#include "layers/ControlLayer.h"

#include "core/EntryPoint.h"

class PlayerApp : public Wyrd::Application
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

Wyrd::Application* Wyrd::CreateApplication()
{
	AppProps properties = AppProps();
	return new PlayerApp(properties);
}

void Wyrd::OnPreAppCreation(Wyrd::Application* application) { }