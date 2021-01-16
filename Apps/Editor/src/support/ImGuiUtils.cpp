#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/renderer/Texture.h>

/* local includes */
#include "ImGuiUtils.h"

namespace ImGui 
{
	void ImGui::Icon(std::shared_ptr<Osiris::Editor::Icon> icon, ImVec2& size)
	{
		std::shared_ptr<Osiris::Editor::TextureRes> texture = icon->iconSet->Texture;
		ImGui::Image((ImTextureID)(INT_PTR)texture->GetTexture()->GetHandle(), size, ImVec2(icon->uv[0].x, icon->uv[0].y), ImVec2(icon->uv[2].x, icon->uv[2].y), ImVec4(1, 1, 1, 1), ImVec4(0, 0, 0, 1));
	}

	bool ImGui::IconButton(std::shared_ptr<Osiris::Editor::Icon> icon, ImGuiID id, bool enabled, ImVec2& size, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
	{
		std::shared_ptr<Osiris::Editor::TextureRes> texture = icon->iconSet->Texture;

		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiButtonFlags flags = 0;
		ImU32 enabledTint = GetColorU32(tint_col);

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;


		if (enabled == false) flags |= ImGuiButtonFlags_Disabled;

		// Default to using texture ID as ID. User can still push string/integer prefixes.
		// We could hash the size/uv to create a unique ID but that would prevent the user from animating UV.
		PushID(id);
		const ImGuiID imgId = window->GetID("#image");
		PopID();

		const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
		const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
		const ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + padding + size);
		ItemSize(bb);
		if (!ItemAdd(bb, imgId))
			return false;

		bool hovered, held;
		bool pressed = ButtonBehavior(bb, imgId, &hovered, &held, flags);

		// Render
		const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);

		if (enabled == false) enabledTint = GetColorU32(ImGuiCol_TabUnfocused);

		RenderNavHighlight(bb, imgId);
		RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
		if (bg_col.w > 0.0f)
			window->DrawList->AddRectFilled(image_bb.Min, image_bb.Max, GetColorU32(bg_col));

		std::shared_ptr<Osiris::Texture> tex = texture->GetTexture();
		if (tex != nullptr)
		{
			ImTextureID tempid = (ImTextureID)(UINT_PTR)tex->GetHandle();
			window->DrawList->AddImage(tempid, image_bb.Min, image_bb.Max, ImVec2(icon->uv[0].x, icon->uv[0].y), ImVec2(icon->uv[2].x, icon->uv[2].y), enabledTint);
		}
		return pressed;
	}


	bool ImGui::TextButton(const char* label, bool enabled, const ImVec2& size_arg, ImGuiButtonFlags flags)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 label_size = CalcTextSize(label, NULL, true);

		ImVec2 pos = window->DC.CursorPos;
		//if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrentLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		//	pos.y += window->DC.CurrentLineTextBaseOffset - style.FramePadding.y;
		ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

		const ImRect bb(pos, pos + size);
		ItemSize(bb, style.FramePadding.y);
		if (!ItemAdd(bb, id))
			return false;

		if (window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat)
			flags |= ImGuiButtonFlags_Repeat;

		if(enabled == false) flags |= ImGuiButtonFlags_Disabled;

		bool hovered, held;
		bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);
		if (pressed)
			MarkItemEdited(id);

		// Render
		ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);

		if (enabled == false) col = GetColorU32(ImGuiCol_TabUnfocused);

		RenderNavHighlight(bb, id);
		RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);
		RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

		return pressed;
	}


	void ImGui::Image(std::shared_ptr<Osiris::Editor::Icon> icon, const ImVec2& size, const ImVec4& tint_col, const ImVec4& border_col)
	{
		std::shared_ptr<Osiris::Editor::TextureRes> texture = icon->iconSet->Texture;

		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return;

		ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
		if (border_col.w > 0.0f)
			bb.Max += ImVec2(2, 2);
		ItemSize(bb);
		if (!ItemAdd(bb, 0))
			return;

		std::shared_ptr<Osiris::Texture> tex = texture->GetTexture();
		ImTextureID tempid = (ImTextureID)(UINT_PTR)tex->GetHandle();

		if (border_col.w > 0.0f)
		{
			window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(border_col), 0.0f);
			window->DrawList->AddImage(tempid, bb.Min + ImVec2(1, 1), bb.Max - ImVec2(1, 1), ImVec2(icon->uv[0].x, icon->uv[0].y), ImVec2(icon->uv[2].x, icon->uv[2].y), GetColorU32(tint_col));
		}
		else
		{
			window->DrawList->AddImage(tempid, bb.Min, bb.Max, ImVec2(icon->uv[0].x, icon->uv[0].y), ImVec2(icon->uv[2].x, icon->uv[2].y), GetColorU32(tint_col));
		}
	}

	/* Vector Outputs */
	void ImGui::LabelVec2(const char* label, glm::vec2& vector, const std::string& xcompLabel, const std::string& ycompLabel)
	{
		ImGui::PushItemWidth(-1);
		ImGui::LabelText("##label", "%s \t%s:%.4f\t%s:%.4f", label, xcompLabel.c_str(), vector.x, ycompLabel.c_str(), vector.y);
		ImGui::PopItemWidth();
	}
}