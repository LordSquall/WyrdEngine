#pragma once

#include "EditorViewDialogBase.h"

#include <imgui.h>

namespace Wyrd::Editor
{
	void EditorViewDialogBase::Close()
	{
		ImGui::CloseCurrentPopup();
	}
}