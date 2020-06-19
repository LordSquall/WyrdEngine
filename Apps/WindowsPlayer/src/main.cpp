/* engine headers */
#include <Osiris.h>

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
	core.open("./Meta.dat", std::ios::in | std::ios::binary);
	if (core.good() == false)
	{
		OSR_CORE_ERROR("Unable to find Meta.dat file");
		return NULL;
	}
	
	AppProps properties = AppProps();
	core >> properties.windowProps.Title;
	return new WindowPlayerApp(properties);
}

void Osiris::OnPreAppCreation(Osiris::Application* application) { }