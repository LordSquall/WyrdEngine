#include "EditorViewBase.h"

#include "support/ImGuiUtils.h"

#include "core/Application.h"
#include "core/Layer.h"

#include <imgui.h>

namespace Wyrd::Editor
{
	void EditorViewBase::OnPreEditorRender()
	{
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;

		/* push all the style/color/setings flags for the window */
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { config.windowPaddingX, config.windowPaddingY });

		/* configure any required flags */
		if (config.allowMenuBar == true) windowFlags = ImGuiWindowFlags_MenuBar;

		/* begin the new window  */
		ImGui::Begin(_Name.c_str());
		ImGui::BeginChild(std::string(_Name + "_child").c_str());
	}

	void EditorViewBase::OnPostEditorRender()
	{
		/* calculate the boundary for the window, this is reqiured to help route mouse events*/
		_Boundary._position = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
		_Boundary._size = { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };

		//{ // Enable for debug viewport overlay
		//	ImVec2 pos = { _Boundary._position.x, _Boundary._position.y };
		//	ImVec2 size = { _Boundary._position.x + _Boundary._size.x, _Boundary._position.y + _Boundary._size.y };
		//	ImGui::GetForegroundDrawList()->AddRect(pos, size, IM_COL32(255, 0, 255, 255));
		//	ImGui::GetForegroundDrawList()->AddCircleFilled(pos, 16.0f, IM_COL32(0, 255, 255, 255));
		//}
		
		/* end the window */
		ImGui::EndChild();
		ImGui::End();

		ImGui::GetWindowContentRegionMax();
		ImGui::GetWindowContentRegionMin();

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