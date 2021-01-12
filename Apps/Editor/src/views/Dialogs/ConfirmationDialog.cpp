#pragma once

#include "osrpch.h"

#include "ConfirmationDialog.h"
#include "imgui.h"

namespace Osiris::Editor
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
			_failureCallback(_dialogData);
			Close();
		}
	}
}