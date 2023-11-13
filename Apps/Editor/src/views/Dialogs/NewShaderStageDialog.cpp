#include "wyrdpch.h"

#include "NewShaderStageDialog.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

namespace Wyrd::Editor
{
	NewShaderStageDialog::NewShaderStageDialog(EditorLayer* editorLayer) : EditorViewDialogBase("Confirm?", editorLayer)
	{

	}

	void NewShaderStageDialog::OnDialogRender()
	{
		ImGui::InputText(_prompt.c_str(), &_name);
		ImGui::RadioButton("Vertex", &_type, 0); ImGui::SameLine();
		ImGui::RadioButton("Fragment", &_type, 1);

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			_successCallback(_name, _type);
			Close();
		}

		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			if (_failureCallback)
				_failureCallback(_name, _type);
			Close();
		}
	}
}