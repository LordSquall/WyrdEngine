#include "wyrdpch.h"

#include "ConfirmationDialog.h"
#include "imgui.h"

namespace Wyrd::Editor
{
	ConfirmationDialog::ConfirmationDialog(EditorLayer* editorLayer) : EditorViewDialogBase("Confirm?", editorLayer)
	{

	}

	void ConfirmationDialog::OnDialogRender()
	{
		ImGui::Text(_message.c_str());

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			_successCallback(_dialogData);
			Close();
		}

		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			if(_failureCallback)
				_failureCallback(_dialogData);
			Close();
		}
	}
}