#include "PrimaryEditorToolbar.h"

#include "support/ImGuiUtils.h"
#include "services/ServiceManager.h"

#include "core/Application.h"
#include "core/Layer.h"

#include <imgui.h>

namespace Wyrd::Editor
{
	PrimaryEditorToolbar::PrimaryEditorToolbar() : EditorToolbarBase()
	{
		auto _resourceService = ServiceManager::Get<ResourceService>();

		/* cache icon resources */
		_2dTranslateIcon = _resourceService->GetIconLibrary().GetIcon("common", "maintools_2d_translate");
		_2dRotateIcon = _resourceService->GetIconLibrary().GetIcon("common", "maintools_2d_rotate");
		_2dScaleIcon = _resourceService->GetIconLibrary().GetIcon("common", "maintools_2d_scale");
	}

	void PrimaryEditorToolbar::OnToolbarRender()
	{
		if (ImGui::IconButton(_2dTranslateIcon, 1, true, ImVec2(16.0f, 16.0f), -1, _SelectedTool == Translate ? ImVec4(0.05f, 0.05f, 0.05f, 1.0f) : ImVec4(0.0f, 0.0f, 0.0f, 0.0f)) == true)
		{
			_SelectedTool = Translate;
		}
		ImGui::SameLine();
		if (ImGui::IconButton(_2dRotateIcon, 2, true, ImVec2(16.0f, 16.0f), -1, _SelectedTool == Rotate ? ImVec4(0.05f, 0.05f, 0.05f, 1.0f) : ImVec4(0.0f, 0.0f, 0.0f, 0.0f)) == true)
		{
			_SelectedTool = Rotate;
		}
		ImGui::SameLine();
		if (ImGui::IconButton(_2dScaleIcon, 3, true, ImVec2(16.0f, 16.0f), -1, _SelectedTool == Scale ? ImVec4(0.05f, 0.05f, 0.05f, 1.0f) : ImVec4(0.0f, 0.0f, 0.0f, 0.0f)) == true)
		{
			_SelectedTool = Scale;
		}
	}
}