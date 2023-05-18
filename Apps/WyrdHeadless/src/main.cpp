/* core wyrd includes */
#include <core/EntryPoint.h>
#include <core/Application.h>

/* local includes */
#include "FrameworkLayer.h"

#ifdef WYRD_RENDERDOC_ENABLED
void* renderdocContext;
#endif

class ClientApplication : public Wyrd::Application
{
public:
	ClientApplication(const Wyrd::AppProps& props) : Application(props)
	{
		PushOverlay(new WyrdHeadless::FrameworkLayer());
	}

	~ClientApplication()
	{

	}
};

Wyrd::Application* Wyrd::CreateApplication()
{
	AppProps properties = AppProps();
	properties.headlessMode = true;

	return new ClientApplication(properties);
}

void Wyrd::OnPreAppCreation(Wyrd::Application* application)
{
#ifdef WYRD_RENDERDOC_ENABLED
	ClientApplication* clientApplication = (ClientApplication*)application;
#endif
}