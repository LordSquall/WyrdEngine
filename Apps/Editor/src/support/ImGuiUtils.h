#pragma once

#include "core/export.h"
#include "support/IconLibrary.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

#define IMGUI_DND_ENTITY	"DND_ENTITY"
#define IMGUI_DND_TEXTURE	"DND_TEXTURE"
#define IMGUI_DND_SCENE		"DND_SCENE"
#define IMGUI_DND_SCRIPT	"DND_SCRIPT"
#define IMGUI_DND_MATERIAL	"DND_MATERIAL"
#define IMGUI_DND_MODEL		"DND_MODEL"

#ifdef WYRD_PLATFORM_LINUX
typedef long INT_PTR;
typedef unsigned long UINT_PTR;
#endif

namespace ImGui
{
	const static ImVec4 BG_COLOR = ImVec4(0, 0, 0, 0);
	const static ImVec4 TINT_COLOR = ImVec4(1, 1, 1, 1);
	
	const static std::string XCOMP_LABEL = "x";
	const static std::string YCOMP_LABEL = "y";

	/* Icon extensions */
	void Icon(const std::shared_ptr<Wyrd::Editor::Icon> icon, const ImVec2& size);

	/* Buttons */
	bool IconButton(std::shared_ptr<Wyrd::Editor::Icon> icon, ImGuiID id, bool enabled, const ImVec2& size, int frame_padding = -1, const ImVec4& bg_col = BG_COLOR, const ImVec4& tint_col = TINT_COLOR);
	bool TextButton(const char* label, bool enabled = true, const ImVec2& size = { 0, 0 }, ImGuiButtonFlags = 0);

	/* Text */
	void TextClipped(const char* text, float clipLength, const ImVec4& text_col, const char* suffix);

	/* Image */
	void Image(const Wyrd::Editor::Icon& icon, const ImVec2& size, const ImVec4& tint_col = TINT_COLOR, const ImVec4& border_col = TINT_COLOR);

	/* Misc */
	bool CollapsingHeader(const Wyrd::Editor::Icon& icon, const char* label, ImGuiTreeNodeFlags flags, const char* label_end = (const char*)0);

	/* Vector Outputs */
	void LabelVec2(const char* label, glm::vec2& vector, const std::string& xcompLabel = XCOMP_LABEL, const std::string& ycompLabel = YCOMP_LABEL);

	/* Hierarchy Objects */
	void TreeSeperator(const int depth);

	/* Callbacks */
	int InputTextCallback(ImGuiInputTextCallbackData* data);
}