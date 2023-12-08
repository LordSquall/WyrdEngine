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
		std::ofstream configFile("config.csv");

		configFile << GetWindow().GetX() << "\n";
		configFile << GetWindow().GetY() << "\n";
		configFile << GetWindow().GetWidth() << "\n";
		configFile << GetWindow().GetHeight() << "\n";
		configFile.close();
	}
};

Wyrd::Application* Wyrd::CreateApplication(int argc, char** argv)
{
	LayerProps props = LayerProps();
	props.windowProps.Title = "ImGui Test App";

	std::ifstream configFile("config.csv");
	
	if (configFile.good())
	{
		std::string line;

		std::getline(configFile, line);
		props.windowProps.X = stoi(line);

		std::getline(configFile, line);
		props.windowProps.Y = stoi(line);

		std::getline(configFile, line);
		props.windowProps.Width = stoi(line);

		std::getline(configFile, line);
		props.windowProps.Height = stoi(line);
	}

	return new ImGuiTestApp(props);
}

void Wyrd::OnPreAppCreation(Wyrd::Application* application) { }