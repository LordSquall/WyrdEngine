/* engine headers */
#include <core/Application.h>

/* local headers */
#include "layers/ControlLayer.h"

#include "core/EntryPoint.h"

class PlayerApp : public Wyrd::Application
{
public:
	PlayerApp(const LayerProps& props) : Application(props)
	{
		if (PushLayer(new ControlLayer(props)) == false)
		{
			Close();
		}
	}

	~PlayerApp()
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
		if (argv[count][0] == '-' && argv[count][1] == '-')
		{
			if (strcmp(argv[count], "--gamedir") == 0)
			{
				properties.gameDir = argv[count + 1];
			}
		}
	}

	return new PlayerApp(properties);
}

void Wyrd::OnPreAppCreation(Wyrd::Application* application) { }