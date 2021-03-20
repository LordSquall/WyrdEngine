#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Layer.h>
#include <core/scene/Layer2D.h>
#include <core/scene/SceneLayer2D.h>
#include <core/scene/components/Transform2DComponent.h>
#include <core/scene/components/SpriteComponent.h>
#include <core/scene/components/ScriptComponent.h>
#include <core/scene/components/PhysicsComponent.h>

/* local includes */
#include "SpriteLayerEditor.h"
#include "support/ImGuiUtils.h"

namespace Osiris::Editor
{
	SpriteLayerEditor::SpriteLayerEditor(EditorLayer* editorLayer) : EditorViewBase("Sprite Layer Editor", editorLayer), _SelectedGameObject(nullptr), _SelectedLayer2D(nullptr), _SelectedSceneLayer(nullptr), _SelectedSceneLayerIdx(0) { }

	SpriteLayerEditor::~SpriteLayerEditor() { }

	void SpriteLayerEditor::OnInitialise() 
	{
		_EventService = ServiceManager::Get<EventService>(ServiceManager::Service::Events);
		_WorkspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Service::Workspace);
		_ResourceService = ServiceManager::Get<ResourceService>(ServiceManager::Service::Resources);

		_EventService->Subscribe(Events::EventType::SceneOpened, EVENT_FUNC(SpriteLayerEditor::OnSceneOpened));
		_EventService->Subscribe(Events::EventType::SelectedGameObjectChanged, EVENT_FUNC(SpriteLayerEditor::OnSelectedGameObjectChanged));

		/* cache icons */
		_Layer2DIcon = _ResourceService->GetIconLibrary().GetIcon("common", "layers_2d");
		_GameObjectIcon = _ResourceService->GetIconLibrary().GetIcon("common", "gameobject");
		_LockIcon = _ResourceService->GetIconLibrary().GetIcon("common", "lock");
		_EyeIcon = _ResourceService->GetIconLibrary().GetIcon("common", "eye");
		_LayerRenameIcon = _ResourceService->GetIconLibrary().GetIcon("common", "layer_rename");
	}

	void SpriteLayerEditor::OnEditorRender()
	{
		static char layerName[64] = "LayerName";
		
		if (_WorkspaceService->IsSceneLoaded())
		{
			if (ImGui::CollapsingHeader("Layers", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::BeginChild("_LayerStackViewChild", ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowSize().y * .25f));
				if (ImGui::BeginTable("spriteLayerTable", 2 ))
				{
					ImGui::TableSetupColumn("Name");
					ImGui::TableSetupColumn("UID");
					ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible
					ImGui::TableHeadersRow();

					int rowIndexCnt = 0;
					for (const auto& layer : _WorkspaceService->GetLoadedScene()->GetLayers())
					{
						bool selectedLayer = (_SelectedSceneLayer != nullptr) && _SelectedSceneLayer->GetUID() == layer->GetUID(); 

						ImGui::PushID(&*layer);
						ImGui::TableNextRow(ImGuiTableRowFlags_None, ImGui::GetTextLineHeightWithSpacing() * 1.2f);
						ImGui::TableNextColumn();
						if (ImGui::Selectable(layer->GetName().c_str(), &selectedLayer, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap))
						{
							_SelectedSceneLayer = &*layer;
							_SelectedSceneLayerIdx = rowIndexCnt;
						}

						if (ImGui::BeginPopupContextItem())
						{
							ImGui::Text("%s : %s", layer->GetName().c_str(), layer->GetUID().str().c_str());
							ImGui::EndPopup();
						}

						ImGui::TableNextColumn();
						ImGui::Text("%s", layer->GetUID().str().c_str());
						ImGui::PopID();

						rowIndexCnt++;
					}
					
					ImGui::EndTable();
				}

				ImGui::EndChild();
			}

			/* layer stack  controls */
			if (ImGui::TextButton("UP", (_SelectedSceneLayer != nullptr) && _SelectedSceneLayerIdx > 0))
			{
				/* swap the current layer with the previous */
				std::iter_swap(_WorkspaceService->GetLoadedScene()->GetLayers().begin() + _SelectedSceneLayerIdx, _WorkspaceService->GetLoadedScene()->GetLayers().begin() + _SelectedSceneLayerIdx - 1);
				_SelectedSceneLayerIdx--;
			}
			ImGui::SameLine();
			if (ImGui::TextButton("DOWN", (_SelectedSceneLayer != nullptr) && _SelectedSceneLayerIdx < (_WorkspaceService->GetLoadedScene()->GetLayers().size() - 1)))
			{
				/* swap the current layer with the next */
				std::iter_swap(_WorkspaceService->GetLoadedScene()->GetLayers().begin() + _SelectedSceneLayerIdx, _WorkspaceService->GetLoadedScene()->GetLayers().begin() + _SelectedSceneLayerIdx + 1);
				_SelectedSceneLayerIdx++;
			}
			ImGui::SameLine();
			if (ImGui::TextButton("DUP", _SelectedSceneLayer != nullptr))
			{
				// TODO
			}
			ImGui::SameLine();
			if (ImGui::TextButton("DEL", _SelectedSceneLayer != nullptr))
			{
				/* remove the selected layer */
				_WorkspaceService->GetLoadedScene()->RemoveLayer(_SelectedSceneLayer->GetUID());
				_SelectedSceneLayer = nullptr;
			}
			ImGui::SameLine();
			if (ImGui::TextButton("ADD", true))
			{
				/* create a new layer and add to scene */
				std::unique_ptr<SceneLayer> newSceneLayer = std::make_unique<SceneLayer2D>("New Layer");
				_WorkspaceService->GetLoadedScene()->AddLayer(std::move(newSceneLayer));
			}

			if (ImGui::CollapsingHeader("Layer Hierarchy", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::BeginChild("_LayerViewChild", ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowSize().y * .50f));
				if (_SelectedSceneLayer != nullptr)
				{
					if (!_SelectedSceneLayer->GetGameObjects().empty())
					{	
						for (auto& gameobject : _SelectedSceneLayer->GetGameObjects())
						{
							RenderGameObjectTreeNode(gameobject);
						}
					}
					else
					{
						ImGui::Text("No Scene Objects in Layer");
					}
				}
				else
				{
					ImGui::Text("No Scene Layer Selected");
				}

				ImGui::EndChild();
			}

			/* layer controls */
			if (ImGui::TextButton("ADD_GO", _SelectedSceneLayer != nullptr))
			{
				std::unique_ptr<GameObject> go = std::make_unique<GameObject>("Game Object");
				go->uid = UIDUtils::Create();

				go->transform = std::move(std::make_unique<Transform2DComponent>(&*go));

				/* create a new gameobject and add to current layer */
				_SelectedSceneLayer->AddChild(std::move(go));
			}
		}
	}

	void SpriteLayerEditor::OnSceneOpened(Events::EventArgs& args)
	{
		if (!_WorkspaceService->GetLoadedScene()->GetLayers().empty())
		{
			_SelectedSceneLayer = &*_WorkspaceService->GetLoadedScene()->GetLayers()[0];
			_SelectedSceneLayerIdx = 0;
		}
	}

	void SpriteLayerEditor::OnSelectedGameObjectChanged(Events::EventArgs& args)
	{
		Events::SelectedGameObjectChangedArgs& evtArgs = static_cast<Events::SelectedGameObjectChangedArgs&>(args);

		_SelectedGameObject = evtArgs.gameObject;
	}

	void SpriteLayerEditor::RenderGameObjectTreeNode(std::unique_ptr<GameObject>& gameObject)
	{
		ImGui::PushID(gameObject->uid.str().c_str());

		/* Build treenode flags */
		ImGuiTreeNodeFlags TreeNodeEx_flags = ImGuiTreeNodeFlags_None;
		TreeNodeEx_flags |= ImGuiTreeNodeFlags_OpenOnArrow;
		gameObject->children.size() == 0 ? TreeNodeEx_flags |= ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_None;			/* add children */
		(&*gameObject == _SelectedGameObject) ? TreeNodeEx_flags |= ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None;		/* currently selected */
		
		if (ImGui::TreeNodeEx(gameObject->name.c_str(), TreeNodeEx_flags))
		{
			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("DND_GAMEOBJECT", &gameObject, sizeof(std::shared_ptr<GameObject>), 0);
				ImGui::Text(_DraggingSelectedGameObject->name.c_str());
				ImGui::EndDragDropSource();
			}

			/* clicking functions */
			if (ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemClicked(ImGuiMouseButton_Right))
			{
				_DraggingSelectedGameObject = &*gameObject;
			}

			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text(gameObject->uid.str().c_str());
				ImGui::EndTooltip();

				if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
				{
					if (_DraggingSelectedGameObject)
					{
						_SelectedGameObject = _DraggingSelectedGameObject;
						_DraggingSelectedGameObject = nullptr;

						/* Fire a change of selection event */
						_EventService->Publish(Editor::Events::EventType::SelectedGameObjectChanged, std::make_unique<Events::SelectedGameObjectChangedArgs>(_SelectedGameObject));
					}
				}
			}

			for (auto& go : gameObject->GetGameObjects())
			{
				RenderGameObjectTreeNode(go);
			}

			ImGui::TreePop();
		}
		ImGui::PopID();
	}
}