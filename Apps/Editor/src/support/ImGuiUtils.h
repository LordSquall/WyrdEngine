#pragma once

#include "core/export.h"
#include "support/IconLibrary.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>

namespace ImGui
{
	/* Icon extensions */
	void Icon(std::shared_ptr<Osiris::Editor::Icon> icon, ImVec2& size);
	bool IconButton(std::shared_ptr<Osiris::Editor::Icon> icon, ImVec2& size);

	/* Buttons */
	bool TextButton(const char* label, bool enabled = true, const ImVec2& size = { 0, 0 }, ImGuiButtonFlags = 0);
}