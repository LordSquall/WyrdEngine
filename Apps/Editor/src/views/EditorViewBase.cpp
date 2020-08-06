#pragma once

#include "EditorViewBase.h"

#include "support/ImGuiUtils.h"

#include "core/Application.h"
#include "core/Layer.h"
#include "core/pipeline/SpriteLayer.h"

#include <imgui.h>

namespace Osiris::Editor
{
	void EditorViewBase::OnPreEditorRender()
	{
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;

		/* push all the style/color/setings flags for the window */
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { config.windowPaddingX, config.windowPaddingY });

		/* configure any required flags */
		if (config.allowMenuBar == true) windowFlags |= ImGuiWindowFlags_MenuBar;

		/* begin the new window  */
		ImGui::Begin(_Name.c_str(), 0, windowFlags);
	}

	void EditorViewBase::OnPostEditorRender()
	{
		/* calculate the boundary for the window, this is reqiured to help route mouse events*/
		_Boundary.x = ImGui::GetWindowPos().x;
		_Boundary.y = ImGui::GetWindowPos().y;
		_Boundary.z = ImGui::GetWindowSize().x;
		_Boundary.w = ImGui::GetWindowSize().y;

		// DEBUG: Display a yellow border around each view bounary
		//{
			//ImGui::GetForegroundDrawList()->AddRect({ _Boundary.x, _Boundary.y }, { _Boundary.x + _Boundary.z, _Boundary.y + _Boundary.w }, IM_COL32(255, 255, 0, 255));
		//}

		/* end the window */
		ImGui::End();

		/* pop each of the stack flags */
		ImGui::PopStyleVar();

		/* check if the boundary has changed between renderer */
		if (_Boundary != _PrevBoundary)
		{
			/* call the onResize and update the boundary info */
			OnResize();
			_PrevBoundary = _Boundary;
		}
	}
}