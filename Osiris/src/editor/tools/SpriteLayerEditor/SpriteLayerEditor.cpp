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

		static const SpriteLayer* currentLayer = spriteLayers[0];
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
		
		if (!currentLayer->GetSprites().empty())
		{
			currentSprite = currentLayer->GetSprites()[0];
			ImGui::ListBoxHeader("");
			for (auto item : currentLayer->GetSprites())
			{
				ImGui::PushID(item->GetID());
				bool selectedSprite = (item == currentSprite);
				if (ImGui::Selectable(item->GetName().c_str(), selectedSprite))
				{
					currentSprite = item;
					ServiceManager::Get<EventService>(ServiceManager::Service::Events).Publish(Editor::Events::EventType::SelectedSpriteChanged, Events::SelectedSpriteChangedArgs(currentSprite));
				}
				if (currentSprite)
					ImGui::SetItemDefaultFocus();
				ImGui::PopID();
			}
			ImGui::ListBoxFooter();
		}
		else
		{
			ImGui::Text("No sprites :(");
		}

		ImGui::End();
	}

}