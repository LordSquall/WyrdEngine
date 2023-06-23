/* core wyrd includes */
#include <wyrdpch.h>
#include <core/renderer/Texture.h>

/* local includes */
#include "ImGuiUtils.h"

namespace ImGui 
{
    /* Icon extensions */
	void Icon(const std::shared_ptr<Wyrd::Editor::Icon> icon, const ImVec2& size)
	{
		std::shared_ptr<Wyrd::Editor::TextureRes> texture = icon->iconSet->Texture;
		ImGui::Image((ImTextureID)(INT_PTR)texture->GetTexture()->GetHandle(), size, ImVec2(icon->uv[0].x, icon->uv[0].y), ImVec2(icon->uv[2].x, icon->uv[2].y), ImVec4(1, 1, 1, 1), ImVec4(0, 0, 0, 0));
	}


    /* Buttons */
	bool IconButton(std::shared_ptr<Wyrd::Editor::Icon> icon, ImGuiID id, bool enabled, const ImVec2& size, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
	{
		std::shared_ptr<Wyrd::Editor::TextureRes> texture = icon->iconSet->Texture;

		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiButtonFlags flags = 0;
		ImU32 enabledTint = GetColorU32(tint_col);

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;


		//if (enabled == false) flags |= ImGuiButtonFlags_Disabled;

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
		bool pressed = ButtonBehavior(bb, imgId, &hovered, &held, flags) && enabled;

		// Render
		const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);

		if (enabled == false) enabledTint = GetColorU32(ImGuiCol_TabUnfocused);

		RenderNavHighlight(bb, imgId);
		RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
		if (bg_col.w > 0.0f)
			window->DrawList->AddRectFilled(image_bb.Min, image_bb.Max, GetColorU32(bg_col));

		std::shared_ptr<Wyrd::Texture> tex = texture->GetTexture();
		if (tex != nullptr)
		{
			ImTextureID tempid = (ImTextureID)(UINT_PTR)tex->GetHandle();
			window->DrawList->AddImage(tempid, image_bb.Min, image_bb.Max, ImVec2(icon->uv[0].x, icon->uv[0].y), ImVec2(icon->uv[2].x, icon->uv[2].y), enabledTint);
		}
		return pressed;
	}

	bool TextButton(const char* label, bool enabled, const ImVec2& size_arg, ImGuiButtonFlags flags)
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
                
		bool hovered, held;
		bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags) && enabled;
		if (pressed)
			MarkItemEdited(id);
        
		// Render
		ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
        
		if (enabled == false) col = GetColorU32(ImGuiCol_TabUnfocused);
        
		RenderNavHighlight(bb, id);
		RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);
		RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
        
        if (enabled)
            return pressed;
        else
            return false;
	}

    /* Text */
    void TextClipped(const char* text, float clipLength, const ImVec4& text_col, const char* suffix = "")
    {
        std::string textContent = text;
        const ImVec2 p0 = ImGui::GetItemRectMin();
        const ImVec2 p1 = ImGui::GetItemRectMax();
        const ImVec2 textPosition = ImVec2(p0.x, p1.y);
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        ImVec2 textSize = ImGui::CalcTextSize(text);
        ImVec2 clippedTextSize = ImVec2(clipLength, textSize.y);

        int suffixLength = strlen(suffix);
        if (textSize.x > clippedTextSize.x)
        {
            textContent.replace(textContent.end() - suffixLength, textContent.end(), suffix);
        }

        ImGui::PushClipRect(textPosition, ImVec2(p1.x, p1.y + textSize.y), true);
        draw_list->AddText(textPosition, GetColorU32(text_col), textContent.c_str());
        ImGui::PopClipRect();

        ImRect bb(textPosition, textPosition + clippedTextSize);
        ItemSize(clippedTextSize, 0.0f);
        ItemAdd(bb, 0);
    }

    /* Image */
	void Image(const Wyrd::Editor::Icon& icon, const ImVec2& size, const ImVec4& tint_col, const ImVec4& border_col)
	{
		std::shared_ptr<Wyrd::Editor::TextureRes> texture = icon.iconSet->Texture;

		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return;

		ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
		if (border_col.w > 0.0f)
			bb.Max += ImVec2(2, 2);
		ItemSize(bb);
		if (!ItemAdd(bb, 0))
			return;

		std::shared_ptr<Wyrd::Texture> tex = texture->GetTexture();
		ImTextureID tempid = (ImTextureID)(UINT_PTR)tex->GetHandle();

		if (border_col.w > 0.0f)
		{
			window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(border_col), 0.0f);
			window->DrawList->AddImage(tempid, bb.Min + ImVec2(1, 1), bb.Max - ImVec2(1, 1), ImVec2(icon.uv[0].x, icon.uv[0].y), ImVec2(icon.uv[2].x, icon.uv[2].y), GetColorU32(tint_col));
		}
		else
		{
			window->DrawList->AddImage(tempid, bb.Min, bb.Max, ImVec2(icon.uv[0].x, icon.uv[0].y), ImVec2(icon.uv[2].x, icon.uv[2].y), GetColorU32(tint_col));
		}
	}

	/* Misc */
	bool CollapsingHeader(const Wyrd::Editor::Icon& icon, const char* label, ImGuiTreeNodeFlags flags, const char* label_end)
	{
	    //ImGuiWindow* window = GetCurrentWindow();
        //if (window->SkipItems)
        //    return false;
        //
        //flags |= ImGuiTreeNodeFlags_CollapsingHeader;
        //
        //ImGuiID id = window->GetID(label);
        //
        //ImGuiContext& g = *GImGui;
        //const ImGuiStyle& style = g.Style;
        //const bool display_frame = (flags & ImGuiTreeNodeFlags_Framed) != 0;
        //const ImVec2 padding = (display_frame || (flags & ImGuiTreeNodeFlags_FramePadding)) ? style.FramePadding : ImVec2(style.FramePadding.x, ImMin(window->DC.CurrLineTextBaseOffset, style.FramePadding.y));
        //
        //if (!label_end)
        //    label_end = FindRenderedTextEnd(label);
        //const ImVec2 label_size = CalcTextSize(label, label_end, false);
        //
        //// We vertically grow up to current line height up the typical widget height.
        //const float frame_height = ImMax(ImMin(window->DC.CurrLineSize.y, g.FontSize + style.FramePadding.y * 2), label_size.y + padding.y * 2);
        //ImRect frame_bb;
        //frame_bb.Min.x = (flags & ImGuiTreeNodeFlags_SpanFullWidth) ? window->WorkRect.Min.x : window->DC.CursorPos.x;
        //frame_bb.Min.y = window->DC.CursorPos.y;
        //frame_bb.Max.x = window->WorkRect.Max.x;
        //frame_bb.Max.y = window->DC.CursorPos.y + frame_height;
        //if (display_frame)
        //{
        //    // Framed header expand a little outside the default padding, to the edge of InnerClipRect
        //    // (FIXME: May remove this at some point and make InnerClipRect align with WindowPadding.x instead of WindowPadding.x*0.5f)
        //    frame_bb.Min.x -= IM_FLOOR(window->WindowPadding.x * 0.5f - 1.0f);
        //    frame_bb.Max.x += IM_FLOOR(window->WindowPadding.x * 0.5f);
        //}
        //
        //const float text_offset_x = g.FontSize + (display_frame ? padding.x * 3 : padding.x * 2);           // Collapser arrow width + Spacing
        //const float text_offset_y = ImMax(padding.y, window->DC.CurrLineTextBaseOffset);                    // Latch before ItemSize changes it
        //const float text_width = g.FontSize + (label_size.x > 0.0f ? label_size.x + padding.x * 2 : 0.0f);  // Include collapser
        //ImVec2 text_pos(window->DC.CursorPos.x + text_offset_x, window->DC.CursorPos.y + text_offset_y);
        //ItemSize(ImVec2(text_width, frame_height), padding.y);
        //
        //// For regular tree nodes, we arbitrary allow to click past 2 worth of ItemSpacing
        //ImRect interact_bb = frame_bb;
        //if (!display_frame && (flags & (ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth)) == 0)
        //    interact_bb.Max.x = frame_bb.Min.x + text_width + style.ItemSpacing.x * 2.0f;
        //
        //// Store a flag for the current depth to tell if we will allow closing this node when navigating one of its child.
        //// For this purpose we essentially compare if g.NavIdIsAlive went from 0 to 1 between TreeNode() and TreePop().
        //// This is currently only support 32 level deep and we are fine with (1 << Depth) overflowing into a zero.
        //const bool is_leaf = (flags & ImGuiTreeNodeFlags_Leaf) != 0;
        //bool is_open = TreeNodeBehaviorIsOpen(id, flags);
        //if (is_open && !g.NavIdIsAlive && (flags & ImGuiTreeNodeFlags_NavLeftJumpsBackHere) && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
        //    window->DC.TreeJumpToParentOnPopMask |= (1 << window->DC.TreeDepth);
        //
        //bool item_add = ItemAdd(interact_bb, id);
        //window->DC.LastItemStatusFlags |= ImGuiItemStatusFlags_HasDisplayRect;
        //window->DC.LastItemDisplayRect = frame_bb;
        //
        //if (!item_add)
        //{
        //    if (is_open && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
        //        TreePushOverrideID(id);
        //    IMGUI_TEST_ENGINE_ITEM_INFO(window->DC.LastItemId, label, window->DC.ItemFlags | (is_leaf ? 0 : ImGuiItemStatusFlags_Openable) | (is_open ? ImGuiItemStatusFlags_Opened : 0));
        //    return is_open;
        //}
        //
        //ImGuiButtonFlags button_flags = ImGuiTreeNodeFlags_None;
        //if (flags & ImGuiTreeNodeFlags_AllowItemOverlap)
        //    button_flags |= ImGuiButtonFlags_AllowItemOverlap;
        //if (!is_leaf)
        //    button_flags |= ImGuiButtonFlags_PressedOnDragDropHold;
        //
        //// We allow clicking on the arrow section with keyboard modifiers held, in order to easily
        //// allow browsing a tree while preserving selection with code implementing multi-selection patterns.
        //// When clicking on the rest of the tree node we always disallow keyboard modifiers.
        //const float arrow_hit_x1 = (text_pos.x - text_offset_x) - style.TouchExtraPadding.x;
        //const float arrow_hit_x2 = (text_pos.x - text_offset_x) + (g.FontSize + padding.x * 2.0f) + style.TouchExtraPadding.x;
        //const bool is_mouse_x_over_arrow = (g.IO.MousePos.x >= arrow_hit_x1 && g.IO.MousePos.x < arrow_hit_x2);
        //if (window != g.HoveredWindow || !is_mouse_x_over_arrow)
        //    button_flags |= ImGuiButtonFlags_NoKeyModifiers;
        //
        //// Open behaviors can be altered with the _OpenOnArrow and _OnOnDoubleClick flags.
        //// Some alteration have subtle effects (e.g. toggle on MouseUp vs MouseDown events) due to requirements for multi-selection and drag and drop support.
        //// - Single-click on label = Toggle on MouseUp (default, when _OpenOnArrow=0)
        //// - Single-click on arrow = Toggle on MouseDown (when _OpenOnArrow=0)
        //// - Single-click on arrow = Toggle on MouseDown (when _OpenOnArrow=1)
        //// - Double-click on label = Toggle on MouseDoubleClick (when _OpenOnDoubleClick=1)
        //// - Double-click on arrow = Toggle on MouseDoubleClick (when _OpenOnDoubleClick=1 and _OpenOnArrow=0)
        //// It is rather standard that arrow click react on Down rather than Up.
        //// We set ImGuiButtonFlags_PressedOnClickRelease on OpenOnDoubleClick because we want the item to be active on the initial MouseDown in order for drag and drop to work.
        //if (is_mouse_x_over_arrow)
        //    button_flags |= ImGuiButtonFlags_PressedOnClick;
        //else if (flags & ImGuiTreeNodeFlags_OpenOnDoubleClick)
        //    button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick;
        //else
        //    button_flags |= ImGuiButtonFlags_PressedOnClickRelease;
        //
        //bool selected = (flags & ImGuiTreeNodeFlags_Selected) != 0;
        //const bool was_selected = selected;
        //
        //bool hovered, held;
        //bool pressed = ButtonBehavior(interact_bb, id, &hovered, &held, button_flags);
        //bool toggled = false;
        //if (!is_leaf)
        //{
        //    if (pressed && g.DragDropHoldJustPressedId != id)
        //    {
        //        if ((flags & (ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick)) == 0 || (g.NavActivateId == id))
        //            toggled = true;
        //        if (flags & ImGuiTreeNodeFlags_OpenOnArrow)
        //            toggled |= is_mouse_x_over_arrow && !g.NavDisableMouseHover; // Lightweight equivalent of IsMouseHoveringRect() since ButtonBehavior() already did the job
        //        if ((flags & ImGuiTreeNodeFlags_OpenOnDoubleClick) && g.IO.MouseDoubleClicked[0])
        //            toggled = true;
        //    }
        //    else if (pressed && g.DragDropHoldJustPressedId == id)
        //    {
        //        IM_ASSERT(button_flags & ImGuiButtonFlags_PressedOnDragDropHold);
        //        if (!is_open) // When using Drag and Drop "hold to open" we keep the node highlighted after opening, but never close it again.
        //            toggled = true;
        //    }
        //
        //    if (g.NavId == id && g.NavMoveRequest && g.NavMoveDir == ImGuiDir_Left && is_open)
        //    {
        //        toggled = true;
        //        NavMoveRequestCancel();
        //    }
        //    if (g.NavId == id && g.NavMoveRequest && g.NavMoveDir == ImGuiDir_Right && !is_open) // If there's something upcoming on the line we may want to give it the priority?
        //    {
        //        toggled = true;
        //        NavMoveRequestCancel();
        //    }
        //
        //    if (toggled)
        //    {
        //        is_open = !is_open;
        //        window->DC.StateStorage->SetInt(id, is_open);
        //        window->DC.LastItemStatusFlags |= ImGuiItemStatusFlags_ToggledOpen;
        //    }
        //}
        //if (flags & ImGuiTreeNodeFlags_AllowItemOverlap)
        //    SetItemAllowOverlap();
        //
        //// In this branch, TreeNodeBehavior() cannot toggle the selection so this will never trigger.
        //if (selected != was_selected) //-V547
        //    window->DC.LastItemStatusFlags |= ImGuiItemStatusFlags_ToggledSelection;
        //
        //// Render
        //const ImU32 text_col = GetColorU32(ImGuiCol_Text);
        //ImGuiNavHighlightFlags nav_highlight_flags = ImGuiNavHighlightFlags_TypeThin;
        //if (display_frame)
        //{
        //    // Framed type
        //    const ImU32 bg_col = GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
        //    RenderFrame(frame_bb.Min, frame_bb.Max, bg_col, true, style.FrameRounding);
        //    RenderNavHighlight(frame_bb, id, nav_highlight_flags);
        //    
        //    ImTextureID textureID = (ImTextureID)(UINT_PTR)icon.iconSet->Texture->GetTexture()->GetHandle();
        //    window->DrawList->AddImage(textureID, frame_bb.Min, frame_bb.Min + ImVec2(16.0f, 16.0f), ImVec2(icon.uv[0].x, icon.uv[0].y), ImVec2(icon.uv[2].x, icon.uv[2].y));
        //
        //    if (flags & ImGuiTreeNodeFlags_Bullet)
        //        RenderBullet(window->DrawList, ImVec2(text_pos.x - text_offset_x * 0.60f, text_pos.y + g.FontSize * 0.5f), text_col);
        //    else if (!is_leaf)
        //        RenderArrow(window->DrawList, ImVec2(text_pos.x - text_offset_x + padding.x, text_pos.y), text_col, is_open ? ImGuiDir_Down : ImGuiDir_Right, 1.0f);
        //    else // Leaf without bullet, left-adjusted text
        //        text_pos.x -= text_offset_x;
        //    if (flags & ImGuiTreeNodeFlags_ClipLabelForTrailingButton)
        //        frame_bb.Max.x -= g.FontSize + style.FramePadding.x;
        //
        //    if (g.LogEnabled)
        //        LogSetNextTextDecoration("###", "###");
        //    RenderTextClipped(text_pos, frame_bb.Max, label, label_end, &label_size);
        //}
        //else
        //{
        //    // Unframed typed for tree nodes
        //    if (hovered || selected)
        //    {
        //        const ImU32 bg_col = GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
        //        RenderFrame(frame_bb.Min, frame_bb.Max, bg_col, false);
        //        RenderNavHighlight(frame_bb, id, nav_highlight_flags);
        //    }
        //
        //    ImRect bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(16.0f, 16.0f));
        //    ItemSize(bb);
        //    ItemAdd(bb, 0);
        //
        //    ImTextureID textureID = (ImTextureID)(UINT_PTR)icon.iconSet->Texture->GetTexture()->GetHandle();
        //    window->DrawList->AddImage(textureID, bb.Min, bb.Min + ImVec2(icon.width, icon.height), ImVec2(icon.uv[0].x, icon.uv[0].y), ImVec2(icon.uv[2].x, icon.uv[2].y));
        //    
        //    if (flags & ImGuiTreeNodeFlags_Bullet)
        //        RenderBullet(window->DrawList, ImVec2(text_pos.x - text_offset_x * 0.5f, text_pos.y + g.FontSize * 0.5f), text_col);
        //    else if (!is_leaf)
        //        RenderArrow(window->DrawList, ImVec2(text_pos.x - text_offset_x + padding.x, text_pos.y + g.FontSize * 0.15f), text_col, is_open ? ImGuiDir_Down : ImGuiDir_Right, 0.70f);
        //    if (g.LogEnabled)
        //        LogSetNextTextDecoration(">", NULL);
        //    RenderText(text_pos, label, label_end, false);
        //}
        //
        //if (is_open && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
        //    TreePushOverrideID(id);
        //IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags | (is_leaf ? 0 : ImGuiItemStatusFlags_Openable) | (is_open ? ImGuiItemStatusFlags_Opened : 0));
        //return is_open;

        return true;
	}

	/* Vector Outputs */
	void LabelVec2(const char* label, glm::vec2& vector, const std::string& xcompLabel, const std::string& ycompLabel)
	{
		ImGui::PushItemWidth(-1);
		ImGui::LabelText("##label", "%s \t%s:%.4f\t%s:%.4f", label, xcompLabel.c_str(), vector.x, ycompLabel.c_str(), vector.y);
		ImGui::PopItemWidth();
	}

    /* Hierarchy Objects */
    void TreeSeperator(int depth)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return;

        ImGuiContext& g = *GImGui;
        
        float thickness_draw = 4.0f;
        float thickness_layout = 1.0f;

        // Horizontal Separator
        float x1 = window->Pos.x;
        float x2 = window->Pos.x + window->Size.x;

        // FIXME-WORKRECT: old hack (#205) until we decide of consistent behavior with WorkRect/Indent and Separator
        if (g.GroupStack.Size > 0 && g.GroupStack.back().WindowID == window->ID)
            x1 += window->DC.Indent.x;

        // FIXME-WORKRECT: In theory we should simply be using WorkRect.Min.x/Max.x everywhere but it isn't aesthetically what we want,
        // need to introduce a variant of WorkRect for that purpose. (#4787)
        if (ImGuiTable* table = g.CurrentTable)
        {
            x1 = table->Columns[table->CurrentColumn].MinX;
            x2 = table->Columns[table->CurrentColumn].MaxX;
        }

        ImGuiOldColumns* columns = window->DC.CurrentColumns;
        if (columns)
            PushColumnsBackground();

        // We don't provide our width to the layout so that it doesn't get feed back into AutoFit
        // FIXME: This prevents ->CursorMaxPos based bounding box evaluation from working (e.g. TableEndCell)
        const ImRect bb(ImVec2(x1, window->DC.CursorPos.y), ImVec2(x2, window->DC.CursorPos.y + thickness_draw));
        ItemSize(ImVec2(0.0f, thickness_layout));
        const bool item_visible = ItemAdd(bb, 0);
        if (item_visible)
        {
            // Draw
            window->DrawList->AddLine({ bb.Min.x + (15 * depth), bb.Min.y}, ImVec2(bb.Max.x, bb.Min.y), GetColorU32(ImGuiCol_Separator));
            if (g.LogEnabled)
                LogRenderedText(&bb.Min, "--------------------------------\n");

        }
        if (columns)
        {
            PopColumnsBackground();
            columns->LineMinY = window->DC.CursorPos.y;
        }
    }

	/* Callbacks */
	int InputTextCallback(ImGuiInputTextCallbackData* data)
	{
		if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
		{
			// Resize string callback
			std::string* str = (std::string*)data->UserData;
			IM_ASSERT(data->Buf == str->c_str());
			str->resize(data->BufTextLen);
			data->Buf = (char*)str->c_str();
		}
		return data->BufTextLen;
	}
}