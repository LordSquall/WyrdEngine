#pragma once

#include "core/export.h"
#include "support/IconLibrary.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

#define IMGUI_DND_TEXTURE	"DND_TEXTURE"
#define IMGUI_DND_SCENE		"DND_SCENE"
#define IMGUI_DND_SCRIPT	"DND_SCRIPT"

namespace ImGui
{
	/* Icon extensions */
	void Icon(std::shared_ptr<Wyrd::Editor::Icon> icon, ImVec2& size);

	/* Buttons */
	bool IconButton(std::shared_ptr<Wyrd::Editor::Icon> icon, ImGuiID id, bool enabled = 1, ImVec2& size = ImVec2(16.0f, 16.0f), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));
	bool TextButton(const char* label, bool enabled = true, const ImVec2& size = { 0, 0 }, ImGuiButtonFlags = 0);

	/* Text */
	void TextClipped(const char* text, float clipLength, const ImVec4& text_col, const char* suffix);

	/* Image */
	void Image(const Wyrd::Editor::Icon& icon, const ImVec2& size, const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(1, 1, 1, 1));

	/* Misc */
	bool CollapsingHeader(const Wyrd::Editor::Icon& icon, const char* label, ImGuiTreeNodeFlags flags, const char* label_end = (const char*)0);

	/* Vector Outputs */
	void LabelVec2(const char* label, glm::vec2& vector, const std::string& xcompLabel = "x", const std::string& ycompLabel = "y");

	/* Callbacks */
	int InputTextCallback(ImGuiInputTextCallbackData* data);
}