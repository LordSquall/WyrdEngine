#include "wyrdpch.h"

#include <core/Log.h>
#include "NewProjectDialog.h"
#include "support/ImGuiUtils.h"

namespace Wyrd::Editor
{
	NewProjectDialog::NewProjectDialog(EditorLayer* editorLayer) : EditorViewDialogBase("New Project", editorLayer)
	{

	}

	void NewProjectDialog::OnDialogRender()
	{
		static std::string projectName;
		static std::string initialSceneName = "default";
		static std::string folderName;
		static bool setupValid = false;

		/* create final directory */
		std::string projectDir = (std::filesystem::path(folderName) / std::filesystem::path(projectName)).string();

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

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Name:");
		ImGui::SameLine();
		ImGui::PushID(0);
		ImGui::InputText("##hidelabel", &projectName);
		ImGui::PopID();

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Default Scene Name:");
		ImGui::SameLine();
		ImGui::PushID(1);
		ImGui::InputText("##hidelabel", &initialSceneName);
		ImGui::PopID();

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Folder:");
		ImGui::SameLine();
		ImGui::PushID(2);
		ImGui::InputText("##hidelabel", &folderName);
		ImGui::PopID();
		ImGui::SameLine();

		if (ImGui::Button("..."))
		{
			folderName = Utils::OpenFolderDialog();
		}

		if (ImGui::TextButton("OK", setupValid, ImVec2(120, 0)))
		{
			ServiceManager::Get<EventService>()->Publish(Events::EventType::CreateNewProject, std::make_unique<Events::CreateNewProjectArgs>(projectName, initialSceneName, folderName));
			ImGui::CloseCurrentPopup();
			Close();
		}

		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ServiceManager::Get<EventService>()->Publish(Events::EventType::CloseEditor, std::make_unique<Events::CloseEditorArgs>());
			ImGui::CloseCurrentPopup();
			Close();
		}
	}
}