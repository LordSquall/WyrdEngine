#pragma once

#include "core/export.h"
#include "support/IconLibrary.h"

#include <imgui.h>

namespace ImGui
{
	void Icon(std::shared_ptr<Osiris::Editor::Icon> icon, ImVec2& size);
	bool IconButton(std::shared_ptr<Osiris::Editor::Icon> icon, ImVec2& size);
}