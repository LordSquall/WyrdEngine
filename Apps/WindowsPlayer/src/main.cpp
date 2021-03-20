/* engine headers */
#include <core/Application.h>

/* local headers */
#include "layers/PrimaryLayer.h"

#include "core/EntryPoint.h"

class WindowPlayerApp : public Osiris::Application
{
public:
	WindowPlayerApp(const AppProps& props) : Application(props)
	{
		if (PushLayer(new PrimaryLayer()) == false)
		{
			Close();
		}
	}

	~WindowPlayerApp()
	{
	}
};

Osiris::Application* Osiris::CreateApplication()
{
	/* open the meta data file */
	std::ifstream core;
	core.open("C:/Projects/GameProjects/DemoProject/.builds/Core.dat", std::ios::in | std::ios::binary);
	if (core.good() == false)
	{
		OSR_CORE_ERROR("Unable to find core.dat file");
		return NULL;
	}
	core.close();

	std::ifstream game;
	game.open("C:/Projects/GameProjects/DemoProject/.builds/Game.dat", std::ios::in | std::ios::binary);
	if (game.good() == false)
	{
		OSR_CORE_ERROR("Unable to find game.dat file");
		return NULL;
	}
	
	AppProps properties = AppProps();
	game >> properties.windowProps.Title;

	game.close();

	return new WindowPlayerApp(properties);
}

void Osiris::OnPreAppCreation(Osiris::Application* application) { }