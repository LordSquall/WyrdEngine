/* engine headers */
#include <core/Application.h>

/* local headers */
#include "layers/TestLayer.h"

#include "core/EntryPoint.h"

class ESCTestApp : public Wyrd::Application
{
public:
	ESCTestApp(const AppProps& props) : Application(props)
	{
		if (PushLayer(new TestLayer()) == false)
		{
			Close();
		}
	}

	~ESCTestApp() = default;
};

Wyrd::Application* Wyrd::CreateApplication()
{
	AppProps properties = AppProps();
	properties.windowProps.Title = "ESC Test App";
	properties.windowProps.Width = 400;
	properties.windowProps.Height = 300;
	properties.windowProps.X = 0;
	properties.windowProps.Y = 28;
	return new ESCTestApp(properties);
}

void Wyrd::OnPreAppCreation(Wyrd::Application* application) { }