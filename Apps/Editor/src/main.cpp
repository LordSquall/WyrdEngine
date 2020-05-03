/* engine headers */
#include <Osiris.h>

#include "layers/Renderer2DLayer.h"
#include "core/EntryPoint.h"

/* local headers */
#include "layers/EditorLayer.h"
#include "layers/EditorRenderer2DLayer.h"
#include "support/IniParser.h"

class ClientApplication : public Osiris::Application
{
public:
	ClientApplication(const AppProps& props) : Application(props)
	{
		std::string defaultProject;

		IniParser iniParser = IniParser("config.ini");

		if (iniParser.IsLoaded())
		{
			defaultProject = iniParser.GetValue("Project", "default", "");
		}

		PushLayer(new Osiris::Editor::EditorRenderer2DLayer("Editor2DLayer"));
		PushOverlay(new Osiris::Editor::EditorLayer(defaultProject));
	}

	~ClientApplication()
	{
		IniParser iniParser = IniParser("config.ini");

		iniParser.SetValue(std::to_string(m_Window->GetWidth()), "Window", "width");
		iniParser.SetValue(std::to_string(m_Window->GetHeight()), "Window", "height");
		iniParser.SetValue(std::to_string(m_Window->GetX()), "Window", "x");
		iniParser.SetValue(std::to_string(m_Window->GetY()), "Window", "y");

		iniParser.Save("config.ini");
	}
};

Osiris::Application* Osiris::CreateApplication()
{
	AppProps properties = AppProps();

	IniParser iniParser = IniParser("config.ini");

	if (iniParser.IsLoaded())
	{
		properties.windowProps.Width = std::stoul(iniParser.GetValue("Window", "width", "800"), NULL, 10);
		properties.windowProps.Height = std::stoul(iniParser.GetValue("Window", "height", "600"), NULL, 10);
		properties.windowProps.X = std::stol(iniParser.GetValue("Window", "x", "25"), NULL, 10);
		properties.windowProps.Y = std::stol(iniParser.GetValue("Window", "y", "25"), NULL, 10);
	}

	return new ClientApplication(properties);
}