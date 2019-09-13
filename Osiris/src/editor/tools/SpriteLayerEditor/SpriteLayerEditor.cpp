#pragma once

#include "osrpch.h"

#include "SpriteLayerEditor.h"

#include "editor/services/ServiceManager.h"
#include "core/Application.h"
#include "core/Layer.h"


#include "imgui.h"

namespace Osiris::Editor
{
	SpriteLayerEditor::SpriteLayerEditor() : EditorPlugin("Sprite Layer Editor"), _SelectedSprite(-1) {}

	SpriteLayerEditor::~SpriteLayerEditor() {}

	void SpriteLayerEditor::OnEditorRender()
	{
		ImGui::Begin("Sprite Layer Editor");

		/* Sprite Layer selection combobox */
		std::vector<SpriteLayer*> spriteLayers = _RenderingLayer->GetSpriteLayers();

		static SpriteLayer* currentLayer = spriteLayers[0];
		if (ImGui::BeginCombo("", currentLayer->GetName().c_str(), 0)) // The second parameter is the label previewed before opening the combo.
		{
			for (int n = 0; n < spriteLayers.size(); n++)
			{
				bool selectedLayer = (currentLayer == spriteLayers[n]);
				if (ImGui::Selectable(spriteLayers[n]->GetName().c_str(), selectedLayer))
					currentLayer = spriteLayers[n];
				if (selectedLayer)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::SameLine();
		if (ImGui::Button("+") == true)
		{
			_RenderingLayer->AddSpriteLayer("New Layer " + std::to_string(_RenderingLayer->GetSpriteLayers().size()));
		}

		/* Sprite Layer List */
		static const Sprite* currentSprite;
		int spriteIdx = 0;

		if (!currentLayer->GetSprites().empty())
		{
			//currentSprite = currentLayer->GetSprites()[0];
			ImGui::ListBoxHeader("", ImVec2(ImGui::GetWindowWidth(), -ImGui::GetItemsLineHeightWithSpacing()));
			for (auto item : currentLayer->GetSprites())
			{
				ImGui::PushID(item->GetID());
				bool selectedSprite = (item == currentSprite);

				if (ImGui::Selectable(item->GetName().c_str(), selectedSprite))
				{
					currentSprite = item;
					ServiceManager::Get<EventService>(ServiceManager::Service::Events).Publish(Editor::Events::EventType::SelectedSpriteChanged, Events::SelectedSpriteChangedArgs(currentSprite));
				}

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					ImGui::SetDragDropPayload("SPRITE_DND_PAYLOAD", &spriteIdx, sizeof(int));        // Set payload to carry the index of our item (could be anything)
					ImGui::EndDragDropSource();
				}
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SPRITE_DND_PAYLOAD"))
					{
						int payload_n = *(const int*)payload->Data;
						currentLayer->SwapSprites(payload_n, spriteIdx);
					}
					ImGui::EndDragDropTarget();
				}


				/* Sprite list context menu */
				if (ImGui::BeginPopupContextItem("item context menu"))
				{
					if (ImGui::Selectable("Delete"))
					{
						currentLayer->RemoveSprite(spriteIdx);
					}
					ImGui::EndPopup();
				}

				if (currentSprite)
					ImGui::SetItemDefaultFocus();
				ImGui::PopID();


				spriteIdx += 1;
			}
			ImGui::ListBoxFooter();
		}
		else
		{
			ImGui::Text("No sprites :(");
		}
		if (ImGui::Button("Add Sprite...") == true)
		{
			currentLayer->AddSprite(new Sprite("New Sprite", 0, 0, 32, 32));
		}

		ImGui::End();
	}

}