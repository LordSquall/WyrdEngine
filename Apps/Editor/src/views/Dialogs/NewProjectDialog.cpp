#include "wyrdpch.h"

#include <core/Log.h>
#include "NewProjectDialog.h"
#include "support/ImGuiUtils.h"

namespace Wyrd::Editor
{
	NewProjectDialog::NewProjectDialog(EditorLayer* editorLayer) : EditorViewDialogBase("New Project", editorLayer)
	{
		_dialogConfig.alwaysCenter = true;
		_dialogConfig.width = 600;
		_dialogConfig.height = 200;
	}

	void NewProjectDialog::OnDialogRender()
	{
		static std::string projectName;
		static std::string initialSceneName = "default";
		static std::string folderName;
		static bool setupValid = false;

		static std::string confirmTest = "Create";
		
		/* create final directory */
		std::string projectDir = (std::filesystem::path(folderName) / std::filesystem::path(projectName)).string();
		
		static int state = 1;	// default to the 'load' state
		ImGui::RadioButton("New ...", &state, 0); ImGui::SameLine();
		ImGui::RadioButton("Load ... ", &state, 1);
		
		if (state == 0)
		{
			if (ImGui::BeginTable("newProjectTableLayout", 2))
			{
				// We could also set ImGuiTableFlags_SizingFixedFit on the table and all columns will default to ImGuiTableColumnFlags_WidthFixed.
				ImGui::TableSetupColumn("label 1", ImGuiTableColumnFlags_WidthFixed, 100.0f);
				ImGui::TableSetupColumn("label 2", ImGuiTableColumnFlags_None);


				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Name:");
				ImGui::TableSetColumnIndex(1);
				ImGui::PushID(0);
				ImGui::PushItemWidth(-1);
				ImGui::InputText("##hidelabel", &projectName);
				ImGui::PopItemWidth();
				ImGui::PopID();


				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Scene Name:");
				ImGui::TableSetColumnIndex(1);
				ImGui::PushID(1);
				ImGui::PushItemWidth(-1);
				ImGui::InputText("##hidelabel", &initialSceneName);
				ImGui::PopItemWidth();
				ImGui::PopID();

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Folder:");
				ImGui::TableSetColumnIndex(1);
				ImGui::PushID(2);
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 30);
				ImGui::InputText("##hidelabel", &folderName);
				ImGui::PopItemWidth();
				ImGui::PopID();
				ImGui::SameLine();

				if (ImGui::Button("..."))
				{
					folderName = Utils::OpenFolderDialog();
				}

				ImGui::EndTable();

			}
			
			confirmTest = "Create";

			if (projectName != "" &&
				initialSceneName != "" &&
				folderName != "" &&
				projectDir != "" &&
				!Utils::FolderExists(projectDir))
			{
				setupValid = true;
			}
			else
			{
				setupValid = false;
			}
		}
		else if (state == 1)
		{
			if (ImGui::BeginTable("newProjectTableLayout", 2))
			{
				// We could also set ImGuiTableFlags_SizingFixedFit on the table and all columns will default to ImGuiTableColumnFlags_WidthFixed.
				ImGui::TableSetupColumn("label 1", ImGuiTableColumnFlags_WidthFixed, 100.0f);
				ImGui::TableSetupColumn("label 2", ImGuiTableColumnFlags_None);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Project:");
				ImGui::TableSetColumnIndex(1);
				ImGui::PushID(2);
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 30);
				ImGui::InputText("##hidelabel", &projectName);
				ImGui::PopItemWidth();
				ImGui::PopID();
				ImGui::SameLine();

				if (ImGui::Button("..."))
				{
					projectName = Utils::OpenFileDialog(".wyrdproj");
				}

				ImGui::EndTable();
			}
			
			ImGui::Separator();
			ImGui::Text("Recent Projects:");
			if (ImGui::BeginTable("recentProjectsTableLayout", 2))
			{
				ImGui::TableSetupColumn("label 1", ImGuiTableColumnFlags_WidthFixed, 200.0f);
				ImGui::TableSetupColumn("label 2");

				bool isSelected = false;
				for (auto& projectEntry : _RecentProjects.GetMap())
				{
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);

					ImGuiSelectableFlags selectableFlags = ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap;
					if (ImGui::Selectable(projectEntry.second.name.c_str(), isSelected, selectableFlags))
					{
						ServiceManager::Get<EventService>()->Publish(Events::EventType::OpenProject, std::make_unique<Events::OpenProjectArgs>(projectEntry.second.path));
						Close();
					}
					ImGui::TableSetColumnIndex(1);
					ImGui::Text(projectEntry.second.path.c_str());
				}

				ImGui::EndTable();
			}

			confirmTest = "Load";
			
			if (projectName != "")
			{
				setupValid = true;
			}
			else
			{
				setupValid = false;
			}

		}

		ImVec2 availableRegion = ImGui::GetWindowSize();
		ImGui::SetCursorPos(ImVec2(availableRegion.x - 130.0f, availableRegion.y - 30.0f));

		if (ImGui::TextButton("Cancel"))
		{
			ImGui::CloseCurrentPopup();
			Close();
		}

		ImGui::SameLine();
		if (ImGui::TextButton(confirmTest.c_str(), setupValid))
		{
			if (state == 0)
				ServiceManager::Get<EventService>()->Publish(Events::EventType::CreateNewProject, std::make_unique<Events::CreateNewProjectArgs>(projectName, initialSceneName, folderName));
			else if (state == 1)
				ServiceManager::Get<EventService>()->Publish(Events::EventType::OpenProject, std::make_unique<Events::OpenProjectArgs>(projectName));

			ImGui::CloseCurrentPopup();
			Close();
		}
	}
}