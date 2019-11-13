#pragma once

#include "osrpch.h"

#include "SpriteLayerEditor.h"

#include "core/Application.h"
#include "core/Layer.h"
#include "editor/datamodels/components/Transform2DComponent.h"

#include "imgui.h"

namespace Osiris::Editor
{
	SpriteLayerEditor::SpriteLayerEditor() : EditorPlugin("Sprite Layer Editor"), _SelectedSprite(-1), _SelectedLayer2D(nullptr){}

	SpriteLayerEditor::~SpriteLayerEditor() {}

	void SpriteLayerEditor::OnInitialise() 
	{
		_SceneService = ServiceManager::Get<SceneService>(ServiceManager::Service::Scene);
		_ResourceService = ServiceManager::Get<ResourceService>(ServiceManager::Service::Resources);

		ServiceManager::Get<EventService>(ServiceManager::Service::Events)->Subscribe(Editor::Events::SceneOpened, EVENT_FUNC(SpriteLayerEditor::OnSceneOpened));
	}

	void SpriteLayerEditor::OnEditorRender()
	{
		ImGui::Begin("Sprite Layer Editor");

		if (_SceneService->IsSceneLoaded())
		{
			/* retrieve a pointer to the loaded scenes layer data structure */
			auto layers = &_SceneService->GetLoadedScene()->layers2D;

			if (ImGui::BeginCombo("", (_SelectedLayer2D != nullptr) ? _SelectedLayer2D->name.c_str() : NULL, 0))
			{
				for (int n = 0; n < layers->size(); n++)
				{
					bool selectedLayer = (_SelectedLayer2D == layers->at(n));
					if (ImGui::Selectable(layers->at(n)->name.c_str(), selectedLayer))
						_SelectedLayer2D = layers->at(n);
					if (selectedLayer)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			/* Add New Layer button */
			ImGui::SameLine();
			if (ImGui::Button("+") == true)
			{
				std::shared_ptr<Layer2D> newLayer2D = std::make_shared<Layer2D>();
				newLayer2D->name = std::string("New Layer " + std::to_string(layers->size() + 1));

				layers->push_back(newLayer2D);
				_SelectedLayer2D = newLayer2D;
			}

			/* Remove Current selected Layer button - Only want this button to appear if a layer to currently selected */
			if (_SelectedLayer2D != nullptr)
			{
				ImGui::SameLine();
				if (ImGui::Button("-") == true)
				{
					auto target = std::find(layers->begin(), layers->end(), _SelectedLayer2D);
					layers->erase(target);
					_SelectedLayer2D = *layers->begin();
				}
			}

			/* Sprite Layer List */
			int spriteIdx = 0;

			if (_SelectedLayer2D != nullptr && !_SelectedLayer2D->gameObjects.empty())
			{
				ImGui::ListBoxHeader("", ImVec2(ImGui::GetWindowWidth(), -ImGui::GetItemsLineHeightWithSpacing()));
				for (int i = 0; i < _SelectedLayer2D->gameObjects.size(); i++)
				{
					ImGui::PushID(spriteIdx);

					if (ImGui::Selectable(_SelectedLayer2D->gameObjects[i]->name.c_str(), _SelectedSprite == spriteIdx))
					{
						_SelectedSprite = spriteIdx;

						/* Fire a change of selection event */
						ServiceManager::Get<EventService>(ServiceManager::Service::Events)->Publish(Editor::Events::SelectedGameObjectChanged, Events::SelectedGameObjectChangedArgs(_SelectedLayer2D->gameObjects[_SelectedSprite]));
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
							_SelectedLayer2D->SwapSprite(payload_n, spriteIdx);
						}
						ImGui::EndDragDropTarget();
					}

					/* Sprite list context menu */
					if (ImGui::BeginPopupContextItem("item context menu"))
					{
						if (ImGui::Selectable("Delete"))
						{
							_SelectedLayer2D->RemoveSprite(spriteIdx);
							_SelectedSprite--;
						}
						ImGui::EndPopup();
					}

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
				AddSpriteGO();
			}
		}
		ImGui::End();
	}

	void SpriteLayerEditor::OnSceneOpened(Events::EventArgs& args)
	{
		if (_SceneService->GetLoadedScene()->layers2D.size() > 0)
		{
			_SelectedLayer2D = _SceneService->GetLoadedScene()->layers2D[0];
		}
	}


	void SpriteLayerEditor::AddSpriteGO()
	{
		GameObject go;
		go.name = "New Sprite";

		/* add base sprite components */
		go.transform2d = std::make_shared<Transform2DComponent>();
		go.spriteRender = std::make_shared<SpriteComponent>();

		/* setup the default sprite renderer */
		go.spriteRender->Sprite = std::make_shared<Sprite>("New Sprite", 0, 0, 256, 256);

		/* setup the base texture */
		go.spriteRender->BaseTexture = _ResourceService->GetTextureByName("box_01");

		/* link the base texture to the sprite */
		go.spriteRender->Sprite->SetTexture(go.spriteRender->BaseTexture->GetTexture());

		_SelectedLayer2D->AddSprite(go);
	}
}