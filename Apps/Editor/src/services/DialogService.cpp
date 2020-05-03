#pragma once

#include "osrpch.h"
#include "ServiceManager.h"
#include "DialogService.h"
#include "events/EditorEvents.h"
#include "support/Utils.h"
#include "platform/OpenGL/imgui_opengl_renderer.h"

namespace Osiris::Editor
{
	void DialogService::OnCreate() {}

	void DialogService::OnDestroy() {}

	void DialogService::OnGUI() 
	{
		if (_isDialogOpen == true)
		{
			ImGui::OpenPopup("Create New Project...");

			if (ImGui::BeginPopupModal("Create New Project...", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				static char name_buffer[255];
				static char folder_buffer[255];

				ImGui::AlignTextToFramePadding();
				ImGui::Text("Name:");
				ImGui::SameLine();
				ImGui::PushID(0);
				ImGui::InputText("##hidelabel", name_buffer, 255);
				ImGui::PopID();

				ImGui::AlignTextToFramePadding();
				ImGui::Text("Folder:");
				ImGui::SameLine();
				ImGui::PushID(1);
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
					ServiceManager::Get<EventService>(ServiceManager::Events)->Publish(Events::EventType::CreateNewProject, Events::CreateNewProjectArgs(std::string(name_buffer), std::string(folder_buffer)));
					ImGui::CloseCurrentPopup();
					_isDialogOpen = false;
				}

				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(120, 0))) 
				{ 
					ImGui::CloseCurrentPopup();
					_isDialogOpen = false;
				}
				ImGui::EndPopup();
			}
		}
	}

	void DialogService::OpenDialog(Dialogs dialog)
	{
		_isDialogOpen = true;
	}
}
