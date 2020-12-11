#pragma once

#include "EditorViewDialogBase.h"

#include <imgui.h>

namespace Osiris::Editor
{
	void EditorViewDialogBase::Close()
	{
		ImGui::CloseCurrentPopup();
	}
}