#include "osrpch.h"

#include "ImGuiLayer_GameViewer.h"

#include "core/Application.h"
#include "core/Layer.h"

#include "events/Event.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

#include "imgui.h"

namespace Osiris
{

	ImGuiLayer_GameViewer::ImGuiLayer_GameViewer() : ImGuiPlugin("Game Viewer"){}

	ImGuiLayer_GameViewer::~ImGuiLayer_GameViewer(){}

	void ImGuiLayer_GameViewer::OnEditorRender()
	{
		ImGui::Begin("Game Viewer");

		if (ImGui::TreeNode("Basic"))
		{
			static int clicked = 0;
			if (ImGui::Button("Button"))
				clicked++;
			if (clicked & 1)
			{
				ImGui::SameLine();
				ImGui::Text("Thanks for clicking me!");
			}

			static bool check = true;
			ImGui::Checkbox("checkbox", &check);

			static int e = 0;
			ImGui::RadioButton("radio a", &e, 0); ImGui::SameLine();
			ImGui::RadioButton("radio b", &e, 1); ImGui::SameLine();
			ImGui::RadioButton("radio c", &e, 2);

			ImGui::TreePop();
		}

		ImGui::End();
	}

}