#pragma once

#include "osrpch.h"

#include "SpriteLayerEditor.h"

#include "core/Application.h"
#include "core/Layer.h"
#include "datamodels/components/Transform2DComponent.h"

#include "support/ImGuiUtils.h"

namespace Osiris::Editor
{
	SpriteLayerEditor::SpriteLayerEditor() : EditorPlugin("Sprite Layer Editor"), _SelectedSprite(-1), _SelectedLayer2D(nullptr) { }

	SpriteLayerEditor::~SpriteLayerEditor() { }

	void SpriteLayerEditor::OnInitialise() 
	{
		_EventService = ServiceManager::Get<EventService>(ServiceManager::Service::Events);
		_WorkspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Service::Workspace);
		_ResourceService = ServiceManager::Get<ResourceService>(ServiceManager::Service::Resources);

		_EventService->Subscribe(Events::EventType::SceneOpened, EVENT_FUNC(SpriteLayerEditor::OnSceneOpened));
		_EventService->Subscribe(Events::EventType::SelectedGameObjectChanged, EVENT_FUNC(SpriteLayerEditor::OnSelectedGameObjectChanged));

		/* cache icons */
		_LayerRenameIcon = _ResourceService->GetIconLibrary().GetIcon("common", "layer_rename");
	}

	void SpriteLayerEditor::OnEditorRender()
	{
		static char layerName[64] = "LayerName";

		ImGui::Begin("Sprite Layer Editor");

		if (_WorkspaceService->IsSceneLoaded())
		{
			/* retrieve a pointer to the loaded scenes layer data structure */
			auto layers = &_WorkspaceService->GetLoadedScene()->layers2D;

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

				if (ImGui::TextButton("-", (layers->size() > 1)) == true)
				{
					auto target = std::find(layers->begin(), layers->end(), _SelectedLayer2D);
					layers->erase(target);
					_SelectedLayer2D = *layers->begin();
				}

				ImGui::SameLine();

				
				if (ImGui::IconButton(_LayerRenameIcon, ImVec2(16.0f, 16.0f)) == true)
					ImGui::OpenPopup("Rename Layer");

				if (ImGui::BeginPopupModal("Rename Layer", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::InputText(_SelectedLayer2D->name.c_str(), &layerName[0], 64);
					if (ImGui::Button("Confirm"))
					{
						_SelectedLayer2D->name = layerName;
						ImGui::CloseCurrentPopup();
					}
					ImGui::SameLine();
					if (ImGui::Button("Cancel"))
						ImGui::CloseCurrentPopup();
					ImGui::EndPopup();
				}
			}

			/* Sprite Layer List */
			int spriteIdx = 0;

			if (_SelectedLayer2D != nullptr && !_SelectedLayer2D->gameobjects.empty())
			{
				ImGui::ListBoxHeader("", ImVec2(ImGui::GetWindowWidth(), -ImGui::GetItemsLineHeightWithSpacing()));
				for (int i = 0; i < _SelectedLayer2D->gameobjects.size(); i++)
				{
					ImGui::PushID(spriteIdx);

					if (ImGui::Selectable(_SelectedLayer2D->gameobjects[i]->name.c_str(), _SelectedSprite == spriteIdx))
					{
						_SelectedSprite = spriteIdx;

						/* Fire a change of selection event */
						_EventService->Publish(Editor::Events::EventType::SelectedGameObjectChanged, Events::SelectedGameObjectChangedArgs(_SelectedLayer2D->gameobjects[_SelectedSprite]));
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
		if (_WorkspaceService->GetLoadedScene()->layers2D.size() > 0)
		{
			_SelectedLayer2D = _WorkspaceService->GetLoadedScene()->layers2D[0];
		}
	}

	void SpriteLayerEditor::OnSelectedGameObjectChanged(Events::EventArgs& args)
	{
		Events::SelectedGameObjectChangedArgs& evtArgs = static_cast<Events::SelectedGameObjectChangedArgs&>(args);

	}

	void SpriteLayerEditor::AddSpriteGO()
	{
		std::shared_ptr<GameObject> go = std::make_shared<GameObject>("New Sprite");

		/* add base sprite components */
		go->transform2d = std::make_shared<Transform2DComponent>();
		go->spriteRender = std::make_shared<SpriteComponent>(go);

		/* setup the default sprite renderer */
		go->spriteRender->Sprite = std::make_shared<Sprite>("New Sprite", 0, 0, 256, 256);

		/* setup the base texture */
		go->spriteRender->BaseTexture = _ResourceService->GetTextureByName("box_01");

		/* link the base texture to the sprite */
		go->spriteRender->Sprite->SetTexture(go->spriteRender->BaseTexture->GetTexture());

		/* setup initial input area for editor input */
		go->inputArea.x = go->spriteRender->Sprite->GetX();
		go->inputArea.y = go->spriteRender->Sprite->GetY();
		go->inputArea.z = go->spriteRender->Sprite->GetWidth();
		go->inputArea.w = go->spriteRender->Sprite->GetHeight();

		_SelectedLayer2D->AddSprite(go);
	}
}