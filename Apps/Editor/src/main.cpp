/* engine headers */
#include <Osiris.h>

#include "layers/Renderer2DLayer.h"
#include "core/EntryPoint.h"

/* local headers */
#include "services/ServiceManager.h"
#include "layers/RenderDocLayer.h"
#include "layers/EditorLayer.h"
#include "support/IniParser.h"

using namespace Osiris::Editor;

#ifdef OSR_RENDERDOC_ENABLED
void* renderdocContext;
#endif

class ClientApplication : public Osiris::Application
{
public:
	ClientApplication(const AppProps& props) : Application(props)
	{
		std::string defaultProject;

		/* initialise editor services */
		ServiceManager::StartServices();

		_SettingsService = ServiceManager::Get<SettingsService>(ServiceManager::Settings);
				
		/* if we are building with renderdoc installed then we can add the layer in */
#ifdef OSR_RENDERDOC_ENABLED
		PushLayer(RenderDocLayer);
#endif

		PushOverlay(new Osiris::Editor::EditorLayer());
	}

	~ClientApplication()
	{
		_SettingsService->SetSetting(std::to_string(m_Window->GetWidth()), "Window", "width");
		_SettingsService->SetSetting(std::to_string(m_Window->GetHeight()), "Window", "height");
		_SettingsService->SetSetting(std::to_string(m_Window->GetX()), "Window", "x");
		_SettingsService->SetSetting(std::to_string(m_Window->GetY()), "Window", "y");


		/* initialise editor services */
		ServiceManager::EndServices();
	}

#ifdef OSR_RENDERDOC_ENABLED
public:
	RenderDocLayer* RenderDocLayer;
#endif

private:
	std::shared_ptr<SettingsService> _SettingsService;
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

void Osiris::OnPreAppCreation(Osiris::Application* application)
{
#ifdef OSR_RENDERDOC_ENABLED
	ClientApplication* clientApplication = (ClientApplication*)application;
	clientApplication->RenderDocLayer = new Osiris::Editor::RenderDocLayer();
#endif
}