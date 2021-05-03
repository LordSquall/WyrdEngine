#pragma once

#include "wyrdpch.h"

#include "NewScriptDialog.h"
#include "imgui.h"

namespace Wyrd::Editor
{
	NewScriptDialog::NewScriptDialog(EditorLayer* editorLayer, const std::string& folder) : EditorViewDialogBase("New Script", editorLayer), _Folder(folder)
	{

	}

	void NewScriptDialog::OnDialogRender()
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
			/* load the script template */
			std::string scriptTemplate = Utils::ReadFileToString(Utils::GetEditorResFolder() + "\\templates\\Script.cs");

			/* replace the main class name */
			std::string scriptContent = Utils::ReplaceAll(scriptTemplate, "<<CLASS_NAME>>", name_buffer);

			Utils::CreateRawFile(_Folder + "\\" + name_buffer + ".cs", scriptContent);

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