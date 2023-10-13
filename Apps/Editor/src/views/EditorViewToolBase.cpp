#include "EditorViewToolBase.h"

#include <imgui.h>

namespace Wyrd::Editor
{
	void EditorViewToolBase::Close()
	{
		ImGui::CloseCurrentPopup();
	}
}