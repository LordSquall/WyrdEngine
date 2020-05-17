#pragma once

#include "osrpch.h"

#include "ImGuiUtils.h"

namespace ImGui 
{
	void ImGui::Icon(std::shared_ptr<Osiris::Editor::Icon> icon, ImVec2& size)
	{
		std::shared_ptr<Osiris::Editor::TextureRes> texture = icon->iconSet->Texture;
		ImGui::Image((ImTextureID)texture->GetRendererHandle(), size, ImVec2(icon->uv[0].x, icon->uv[0].y), ImVec2(icon->uv[2].x, icon->uv[2].y), ImVec4(1, 1, 1, 1), ImVec4(0, 0, 0, 1));
	}

	bool ImGui::IconButton(std::shared_ptr<Osiris::Editor::Icon> icon, ImVec2& size)
	{
		std::shared_ptr<Osiris::Editor::TextureRes> texture = icon->iconSet->Texture;
		return ImGui::ImageButton((ImTextureID)texture->GetRendererHandle(), size, ImVec2(icon->uv[0].x, icon->uv[0].y), ImVec2(icon->uv[2].x, icon->uv[2].y));
	}
}