#include "osrpch.h"

#include "ImGuiLayer_ResourceViewer.h"

#include "core/Application.h"
#include "core/Layer.h"

#include "events/Event.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

#include "imgui.h"

namespace Osiris
{

	ImGuiLayer_ResourceViewer::ImGuiLayer_ResourceViewer() : ImGuiPlugin("Resource Viewer"){}

	ImGuiLayer_ResourceViewer::~ImGuiLayer_ResourceViewer(){}

	void ImGuiLayer_ResourceViewer::OnEditorRender()
	{
		ImGui::Begin("Resource Viewer");

		for (std::map<std::string, Osiris::Shader>::iterator it = Resources::Get().m_ShaderMap.begin(); it != Resources::Get().m_ShaderMap.end(); ++it)
		{
			ImGui::Text(it->first.c_str());
		}
		ImGui::Text("Welcome to the Resource Viewer Plugin");

		ImGui::End();
	}

}