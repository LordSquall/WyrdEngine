#pragma once

#include "wyrdpch.h"

#include "NewProjectDialog.h"
#include "imgui.h"

namespace Wyrd::Editor
{
	NewProjectDialog::NewProjectDialog(EditorLayer* editorLayer) : EditorViewDialogBase("New Project", editorLayer)
	{

	}

	void NewProjectDialog::OnDialogRender()
	{
		static char name_buffer[255];
		static char scenename_buffer[255] = "default";
		static char folder_buffer[255];

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Name:");
		ImGui::SameLine();
		ImGui::PushID(0);
		ImGui::InputText("##hidelabel", name_buffer, 255);
		ImGui::PopID();

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Default Scene Name:");
		ImGui::SameLine();
		ImGui::PushID(1);
		ImGui::InputText("##hidelabel", scenename_buffer, 255);
		ImGui::PopID();

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Folder:");
		ImGui::SameLine();
		ImGui::PushID(2);
		ImGui::InputText("##hidelabel", folder_buffer, 255);
		ImGui::PopID();
		ImGui::SameLine();

		if (ImGui::Button("..."))
		{
			Utils util;
			strcpy(folder_buffer, util.OpenFolderDialog().c_str());
			printf("Folder: %s\n", folder_buffer);
		}

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			ServiceManager::Get<EventService>(ServiceManager::Events)->Publish(Events::EventType::CreateNewProject, std::make_unique<Events::CreateNewProjectArgs>(std::string(name_buffer), std::string(scenename_buffer), std::string(folder_buffer)));
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