/* core wyrd includes */
#include <wyrdpch.h>

/* local includes */
#include "SpriteSheetImporterDialog.h"
#include "support/ImGuiUtils.h"
#include "loaders/SpriteSheetLoader.h"

namespace Wyrd::Editor
{
	SpriteSheetImporterDialog::SpriteSheetImporterDialog(EditorLayer* editorLayer, TextureRes* textureResource) : EditorViewDialogBase("Sprite Sheet Importer Dialog", editorLayer), _SelectedImageAreaID(-1)
	{
		_TextureResource = textureResource;

		/* check if there is a sprite data file present */
		std::filesystem::path spriteDataFile = _TextureResource->GetPath().parent_path() / (_TextureResource->GetPath().stem().string() + ".spritedata");
		if (Utils::FileExists(spriteDataFile))
		{
			SpriteSheetLoader::Load(spriteDataFile, _SubImageData);
		}
	}

	SpriteSheetImporterDialog::~SpriteSheetImporterDialog() {}

	void SpriteSheetImporterDialog::OnDialogRender()
	{
		static ImVec2 rawUnitSize = { 128, 128 };

		ImVec2 imageRegionSize			= { ImGui::GetContentRegionAvail().x * 0.80f, ImGui::GetContentRegionAvail().y - ImGui::GetItemRectSize().y };
		ImVec2 controlRegionSize		= { ImGui::GetContentRegionAvail().x * 0.20f, ImGui::GetContentRegionAvail().y - ImGui::GetItemRectSize().y };
		ImVec2 controlDataRegionSize	= { ImGui::GetContentRegionAvail().x * 0.20f, imageRegionSize.y - 200.0f };
		ImVec2 controlAutoGenRegionSize = { ImGui::GetContentRegionAvail().x * 0.20f, 200.0f };
		ImVec2 dialogControlsRegionSize = { ImGui::GetContentRegionAvail().x, ImGui::GetItemRectSize().y };

		ImVec2 rawImageSize = { (float)_TextureResource->GetWidth(), (float)_TextureResource->GetHeight() };
		float scaleFactor = std::min<float>(imageRegionSize.x / rawImageSize.x, imageRegionSize.y / rawImageSize.y);

		ImVec2 scaledImageSize = rawImageSize * scaleFactor;
		ImVec2 scaledUnitSize = rawUnitSize * scaleFactor;

		ImGui::BeginChild("ImageRegion", imageRegionSize, false, ImGuiWindowFlags_HorizontalScrollbar);
		ImTextureID texture = (ImTextureID)(INT_PTR)_TextureResource->GetTexture()->GetHandle();

		ImVec2 cursorPos = ImGui::GetCursorScreenPos();

		ImGui::Image(texture, scaledImageSize);

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		for (auto& imageArea : _SubImageData)
		{
			/* calculate absolute pixel positions from UV */
			ImVec2 pt1 = { rawImageSize.x * imageArea.uv1.x, rawImageSize.y * imageArea.uv1.y };
			ImVec2 pt2 = { rawImageSize.x * imageArea.uv2.x, rawImageSize.y * imageArea.uv2.y };

			pt1 += cursorPos;
			pt2 += cursorPos;

			ImU32 outlineColor = IM_COL32(255, 255, 0, 255);

			if (imageArea.id == _SelectedImageAreaID)
			{
				outlineColor = IM_COL32(255, 0, 255, 255);
			}

			drawList->AddRect(pt1, pt2, outlineColor, 1.0f);
		}

		ImGui::EndChild();

		ImGui::SameLine();
		
		ImGui::BeginChild("ControlRegion", controlRegionSize);

		ImGui::Text("Image Areas");

		ImGui::BeginChild("ControlDataRegion", controlDataRegionSize);
		if (ImGui::BeginTable("dataTable", 1, ImGuiTableFlags_SizingFixedFit))
		{
			ImGui::TableSetupColumn("Name");

			for (auto& img : _SubImageData)
			{
				ImGui::PushID(img.id);
				ImGui::TableNextRow(ImGuiTableRowFlags_None, ImGui::GetTextLineHeightWithSpacing() * 1.2f);
				ImGui::TableNextColumn();

				bool selected = img.id == _SelectedImageAreaID;
				if (ImGui::Selectable(img.name.c_str(), &selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap))
				{
					_SelectedImageAreaID = img.id;
				}
				ImGui::PopID();
			}

			ImGui::EndTable();
		}
		ImGui::EndChild();

		ImGui::BeginChild("AutoGenChild", controlAutoGenRegionSize);
		if (ImGui::BeginTabBar("AutoGenTabBar", 0))
		{
			if (ImGui::BeginTabItem("Grid"))
			{
				ImGui::InputFloat2("Unit Size", (float*)&rawUnitSize);
				if (ImGui::Button("Generate"))
				{
					/* remove all old image areas */
					_SubImageData.clear();

					int id = 0;

					/* calculate the number of rows and columns we */
					ImVec2 gridSize = scaledImageSize / scaledUnitSize;
					
					for (int r = 0; r < gridSize.y; r++)
					{
						for (int c = 0; c < gridSize.x; c++)
						{
							Vector2 uv1 = { (float)(scaledUnitSize.x * c) / (float)_TextureResource->GetWidth(), (float)(scaledUnitSize.y * r) / (float)_TextureResource->GetHeight() };

							Vector2 uv2 = { (float)(scaledUnitSize.x * (c + 1)) / (float)_TextureResource->GetWidth(), (float)(scaledUnitSize.y * (r + 1)) / (float)_TextureResource->GetHeight() };

							_SubImageData.push_back({ id, std::to_string(r) + "-" + std::to_string(c), uv1, uv2 });
							id++;
						}
					}
				}
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::EndChild();
		ImGui::EndChild();

		ImGui::BeginChild("DialogControlsRegion", dialogControlsRegionSize);
		if (ImGui::Button("Cancel"))
		{
			Close();
		}
		
		ImGui::SameLine();
		if (ImGui::Button("OK"))
		{
			/* build file name */
			std::filesystem::path spriteDataPath = _TextureResource->GetPath().parent_path() / (_TextureResource->GetPath().stem().string() + ".spritedata");

			/* save the new sprite sheet data file  */
			SpriteSheetLoader::Save(spriteDataPath, _SubImageData);

			Close();
		}
		ImGui::EndChild();
	}
}