#pragma once

#include "wyrdpch.h"

#include "NewSceneDialog.h"
#include "imgui.h"

namespace Wyrd::Editor
{
	NewSceneDialog::NewSceneDialog(EditorLayer* editorLayer, const std::string& folder) : EditorViewDialogBase("New Script", editorLayer), _Folder(folder)
	{

	}

	void NewSceneDialog::OnDialogRender()
	{
		static char name_buffer[255];
		static char folder_buffer[255];

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Name:");
		ImGui::SameLine();
		ImGui::PushID(0);
		ImGui::InputText("##hidelabel", name_buffer, 255);
		ImGui::PopID();

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			/* load the scene template */
			std::string sceneTemplate = Utils::ReadFileToString(Utils::GetEditorResFolder() + "\\templates\\Scene.scene");

			Utils::CreateRawFile(_Folder + "\\" + name_buffer + ".scene", sceneTemplate);

			ImGui::CloseCurrentPopup();
			Close();
		}

		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
			Close();
		}
	}
}