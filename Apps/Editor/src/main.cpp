/* core wyrd includes */
#include <core/EntryPoint.h>
#include <core/Application.h>

/* local includes */
#include "services/ServiceManager.h"
#include "layers/RenderDocLayer.h"
#include "layers/EditorLayer.h"
#include "support/IniParser.h"

using namespace Wyrd::Editor;

#ifdef WYRD_RENDERDOC_ENABLED
void* renderdocContext;
#endif

class ClientApplication : public Wyrd::Application
{
public:
	ClientApplication(const Wyrd::AppProps& props) : Application(props)
	{
		std::string defaultProject;

		/* initialise editor services */
		ServiceManager::StartServices();

		/* if we are building with renderdoc installed then we can add the layer in */
#ifdef WYRD_RENDERDOC_ENABLED
		PushLayer(RenderDocLayer);
#endif

		PushOverlay(new Wyrd::Editor::EditorLayer());
	}

	~ClientApplication()
	{

		std::shared_ptr<SettingsService> settingsService = ServiceManager::Get<SettingsService>(ServiceManager::Settings);
		settingsService->SetSetting(std::to_string(_Window->GetWidth()), CONFIG_WINDOW, CONFIG_WINDOW__WIDTH);
		settingsService->SetSetting(std::to_string(_Window->GetHeight()), CONFIG_WINDOW, CONFIG_WINDOW__HEIGHT);
		settingsService->SetSetting(std::to_string(_Window->GetX()), CONFIG_WINDOW, CONFIG_WINDOW__X);
		settingsService->SetSetting(std::to_string(_Window->GetY()), CONFIG_WINDOW, CONFIG_WINDOW__Y);


		/* initialise editor services */
		ServiceManager::EndServices();
	}

#ifdef WYRD_RENDERDOC_ENABLED
public:
	RenderDocLayer* RenderDocLayer;
#endif

};

Wyrd::Application* Wyrd::CreateApplication()
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

void Wyrd::OnPreAppCreation(Wyrd::Application* application)
{
#ifdef WYRD_RENDERDOC_ENABLED
	ClientApplication* clientApplication = (ClientApplication*)application;
	clientApplication->RenderDocLayer = new Wyrd::Editor::RenderDocLayer();
#endif
}