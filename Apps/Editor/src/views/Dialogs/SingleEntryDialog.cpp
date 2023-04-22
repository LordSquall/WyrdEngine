#include "wyrdpch.h"

#include "SingleEntryDialog.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

namespace Wyrd::Editor
{
	SingleEntryDialog::SingleEntryDialog(EditorLayer* editorLayer) : EditorViewDialogBase("Confirm?", editorLayer)
	{

	}

	void SingleEntryDialog::OnDialogRender()
	{
		ImGui::InputText(_prompt.c_str(), &_content);

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			_successCallback(_content);
			Close();
		}

		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			if(_failureCallback)
				_failureCallback(_content);
			Close();
		}
	}
}