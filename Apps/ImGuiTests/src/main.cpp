/* engine headers */
#include <core/Application.h>

/* local headers */
#include "layers/ImGuiTestLayer.h"

#include "core/EntryPoint.h"

class ImGuiTestApp : public Wyrd::Application
{
public:
	ImGuiTestApp(const LayerProps& props) : Application(props)
	{
		if (PushLayer(new ImGuiTestLayer(props)) == false)
		{
			Close();
		}
	}

	~ImGuiTestApp()
	{
		//system("pause");
	}
};

Wyrd::Application* Wyrd::CreateApplication(int argc, char** argv)
{
	LayerProps properties = LayerProps();

	/* Process command line args */
	for (int count{ 0 }; count < argc; ++count)
	{
		
	}

	return new ImGuiTestApp(properties);
}

void Wyrd::OnPreAppCreation(Wyrd::Application* application) { }