#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Layer.h>
#include <core/scene/Layer2D.h>
#include <core/scene/components/Transform2DComponent.h>
#include <core/scene/components/SpriteComponent.h>
#include <core/scene/components/ScriptComponent.h>
#include <core/scene/components/PhysicsComponent.h>

/* local includes */
#include "SpriteLayerEditor.h"
#include "support/ImGuiUtils.h"

namespace Osiris::Editor
{
	SpriteLayerEditor::SpriteLayerEditor(EditorLayer* editorLayer) : EditorViewBase("Sprite Layer Editor", editorLayer), _SelectedGameObject(nullptr), _SelectedLayer2D(nullptr) { }

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
		
		if (_WorkspaceService->IsSceneLoaded())
		{
			/* retrieve a pointer to the loaded scenes layer data structure */
			auto layers = &_WorkspaceService->GetLoadedScene()->layers2D;

			ImGui::PushID("layer_combobox");
			if (ImGui::BeginCombo("##nolabel", (_SelectedLayer2D != nullptr) ? _SelectedLayer2D->name.c_str() : NULL, 0))
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
			ImGui::PopID();

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

				
				if (ImGui::IconButton(_LayerRenameIcon, 1, true, ImVec2(16.0f, 16.0f)) == true)
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

			ImGui::BeginChild("gameobject_inner");
			if (_SelectedLayer2D != nullptr && !_SelectedLayer2D->children.empty())
			{
				for (int i = 0; i < _SelectedLayer2D->children.size(); i++)
				{
					RenderGameObjectTreeNode(_SelectedLayer2D->children[i]);
				}
			}
			else
			{
				ImGui::Text("No sprites :(");
			}

			/* Sprite list context menu */
			if(ImGui::BeginPopupContextWindow() == true)
			{ 
				if (ImGui::Selectable("Add Child") == true)
				{
					if (_SelectedGameObject != nullptr)
					{
						_SelectedGameObject->AddChild(CreateEmptyGameObject());
					}
					else
					{
						_SelectedLayer2D->AddChild(CreateEmptyGameObject());
					}
				}
				if (ImGui::BeginMenu("Add Component", _SelectedGameObject != nullptr))
				{
					if (ImGui::MenuItem("Sprite") == true)
					{
						_SelectedGameObject->components.push_back(std::make_shared<SpriteComponent>(_SelectedGameObject));
						_EventService->Publish(Editor::Events::EventType::SelectedGameObjectChanged, std::make_shared<Events::SelectedGameObjectChangedArgs>(_SelectedGameObject));
					}
					if (ImGui::MenuItem("Script") == true)
					{
						_SelectedGameObject->components.push_back(std::make_shared<ScriptComponent>(_SelectedGameObject));
						_EventService->Publish(Editor::Events::EventType::SelectedGameObjectChanged, std::make_shared<Events::SelectedGameObjectChangedArgs>(_SelectedGameObject));
					}
					if (ImGui::MenuItem("Physics") == true)
					{
						_SelectedGameObject->components.push_back(std::make_shared<PhysicsComponent>(_SelectedGameObject));
						_EventService->Publish(Editor::Events::EventType::SelectedGameObjectChanged, std::make_shared<Events::SelectedGameObjectChangedArgs>(_SelectedGameObject));
					}

					ImGui::EndMenu();
				}

				ImGui::Separator();
				if (ImGui::MenuItem("Delete") == true)
				{
					if(_SelectedGameObject->parent == nullptr)
						_SelectedLayer2D->RemoveChild(_SelectedGameObject->GetUID());
					else
						_SelectedGameObject->parent->RemoveChild(_SelectedGameObject->GetUID());
				}
				ImGui::EndPopup();
			}
			ImGui::EndChild();
		}
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

		_SelectedGameObject = evtArgs.gameObject;
	}


	void SpriteLayerEditor::RenderGameObjectTreeNode(std::shared_ptr<GameObject> gameObject)
	{
		ImGui::PushID(gameObject->GetUID());

		/* Build treenode flags */
		ImGuiTreeNodeFlags TreeNodeEx_flags = ImGuiTreeNodeFlags_None;
		TreeNodeEx_flags |= ImGuiTreeNodeFlags_OpenOnArrow;
		gameObject->children.size() == 0 ? TreeNodeEx_flags |= ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_None;			/* add children */
		gameObject == _SelectedGameObject ? TreeNodeEx_flags |= ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None;		/* currently selected */
		
		if (ImGui::TreeNodeEx(gameObject->name.c_str(), TreeNodeEx_flags))
		{
			if (ImGui::BeginDragDropSource())
			{
				uint32_t uid = _DraggingSelectedGameObject->GetUID();
				ImGui::SetDragDropPayload("GAMEOBJECT", &uid, sizeof(uint32_t), 0);
				ImGui::Text(_DraggingSelectedGameObject->name.c_str());
				ImGui::EndDragDropSource();
			}

			/* clicking functions */
			if (ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemClicked(ImGuiMouseButton_Right))
			{
				_DraggingSelectedGameObject = gameObject;
			}

			if (ImGui::IsItemHovered())
			{
				if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
				{
					if (_DraggingSelectedGameObject)
					{
						_SelectedGameObject = _DraggingSelectedGameObject;
						_DraggingSelectedGameObject = nullptr;

						/* Fire a change of selection event */
						_EventService->Publish(Editor::Events::EventType::SelectedGameObjectChanged, std::make_shared<Events::SelectedGameObjectChangedArgs>(_SelectedGameObject));
					}
				}
			}

			for (int i = 0; i < gameObject->children.size(); i++)
			{
				RenderGameObjectTreeNode(gameObject->children[i]);
			}

			ImGui::TreePop();
		}
		ImGui::PopID();
	}

	std::shared_ptr<GameObject> SpriteLayerEditor::CreateEmptyGameObject()
	{
		std::shared_ptr<GameObject> go = std::make_shared<GameObject>("Game Object");

		/* add transform 2D */
		go->transform2D = std::make_shared<Transform2DComponent>(go);

		return go;
	}
}