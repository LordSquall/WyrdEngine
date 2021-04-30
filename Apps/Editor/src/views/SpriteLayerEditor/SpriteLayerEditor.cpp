#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Layer.h>
#include <core/scene/Layer2D.h>
#include <core/scene/SceneLayer2D.h>
#include <core/scene/components/SceneComponentFactory.h>
#include <core/KeyCodes.h>

/* local includes */
#include "SpriteLayerEditor.h"
#include "support/ImGuiUtils.h"

namespace Osiris::Editor
{
	SpriteLayerEditor::SpriteLayerEditor(EditorLayer* editorLayer) : EditorViewBase("Sprite Layer Editor", editorLayer), _SelectedGameObject(nullptr), _SelectedLayer2D(nullptr), _SelectedSceneLayer(nullptr), _SelectedSceneLayerIdx(0), _SelectedGameObjectIdx(0), _EditLayerNameMode(0) { }

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
				if (ImGui::BeginTable("spriteLayerTable", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit))
				{
					ImGui::TableSetupColumn("Name");
					ImGui::TableSetupColumn("Actions");
					ImGui::TableSetupScrollFreeze(0, 1);
					ImGui::TableHeadersRow();

					int rowIndexCnt = 0;
					for (const auto& layer : _WorkspaceService->GetLoadedScene()->GetLayers())
					{
						bool selectedLayer = (_SelectedSceneLayer != nullptr) && _SelectedSceneLayer->GetUID() == layer->GetUID(); 

						ImGui::PushID(&*layer);
						ImGui::TableNextRow(ImGuiTableRowFlags_None, ImGui::GetTextLineHeightWithSpacing() * 1.2f);
						ImGui::TableNextColumn();

						if (_EditLayerNameMode)
						{
							ImGui::SetKeyboardFocusHere(0);
							if (ImGui::InputText("##label", &layer->GetName(),
								ImGuiInputTextFlags_CallbackCharFilter | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll, ImGui::InputTextCallback))
							{
								OSR_TRACE("Updated Layer Name!!");
								_EditLayerNameMode = false;
							}
						}
						else
						{
							if (ImGui::Selectable(layer->GetName().c_str(), &selectedLayer, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap))
							{
								_SelectedSceneLayer = &*layer;
								_SelectedSceneLayerIdx = rowIndexCnt;
							}
						}

						if (ImGui::BeginPopupContextItem())
						{	
							if (ImGui::MenuItem("Rename"))
							{
								_EditLayerNameMode = true;
							}
							ImGui::EndPopup();
						}

						ImGui::TableNextColumn();

						// TEMP 
						ImGuiContext& g = *GImGui;
						float backup_padding_y = g.Style.FramePadding.y;
						g.Style.FramePadding.y = 0.0f;
						
						int activeBtnFlags = ImGuiButtonFlags_AlignTextBaseLine;
						if (layer->IsActive())
							activeBtnFlags |= ImGuiButtonFlags_Disabled;

						if (ImGui::ButtonEx("A", ImVec2(0, 0), ImGuiButtonFlags_AlignTextBaseLine))
							layer->SetActive(!layer->IsActive());

						ImGui::SameLine();


						int visibleBtnFlags = ImGuiButtonFlags_AlignTextBaseLine;
						if (layer->IsVisible())
							activeBtnFlags |= ImGuiButtonFlags_Disabled;

						if (ImGui::ButtonEx("V", ImVec2(0, 0), ImGuiButtonFlags_AlignTextBaseLine))
							layer->SetVisible(!layer->IsVisible());

						g.Style.FramePadding.y = backup_padding_y;

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
				newSceneLayer->Initialise();
				_WorkspaceService->GetLoadedScene()->AddLayer(std::move(newSceneLayer));
			}

			if (ImGui::CollapsingHeader("Layer Hierarchy", ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (_SelectedSceneLayer != nullptr)
				{
					if (dynamic_cast<SceneLayer2D*>(_SelectedSceneLayer))
					{
						ImGui::BeginChild("_Layer2DViewChild", ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowSize().y * .50f));

						if (!_SelectedSceneLayer->GetGameObjects().empty())
						{
							int rowIndexCnt = 0;
							for (auto& gameobject : _SelectedSceneLayer->GetGameObjects())
							{
								/* track selected object */
								bool isSelected = (_SelectedGameObject != nullptr) && _SelectedGameObject->uid == gameobject->uid;
								if (ImGui::Selectable(gameobject->name.c_str(), &isSelected))
								{
									_SelectedGameObject = gameobject.get();

									/* fire a change of selection event */
									_EventService->Publish(Editor::Events::EventType::SelectedGameObjectChanged, std::make_unique<Events::SelectedGameObjectChangedArgs>(_SelectedGameObject));

									_SelectedGameObjectIdx = rowIndexCnt;
								}

								if (ImGui::BeginPopupContextItem())
								{
									if (ImGui::BeginMenu("Add Compontents"))
									{
										if (ImGui::MenuItem("Sprite"))
										{
											IBaseComponent* component = gameobject->AddComponent(std::move(SceneComponentFactory::Create(SceneComponentType::SpriteRenderer, gameobject.get())));
											component->Initialise();

											/* fire a change of selection event to refresh the display */
											_EventService->Publish(Editor::Events::EventType::SelectedGameObjectChanged, std::make_unique<Events::SelectedGameObjectChangedArgs>(_SelectedGameObject));
										}

										if (ImGui::MenuItem("Script"))
										{
											IBaseComponent* component = gameobject->AddComponent(std::move(SceneComponentFactory::Create(SceneComponentType::ScriptComponent, gameobject.get())));
											component->Initialise();

											/* fire a change of selection event to refresh the display */
											_EventService->Publish(Editor::Events::EventType::SelectedGameObjectChanged, std::make_unique<Events::SelectedGameObjectChangedArgs>(_SelectedGameObject));
										}

										if (ImGui::MenuItem("Physics"))
										{
											IBaseComponent* component = gameobject->AddComponent(std::move(SceneComponentFactory::Create(SceneComponentType::PhysicsComponent, gameobject.get())));
											component->Initialise();

											/* fire a change of selection event to refresh the display */
											_EventService->Publish(Editor::Events::EventType::SelectedGameObjectChanged, std::make_unique<Events::SelectedGameObjectChangedArgs>(_SelectedGameObject));
										}

										if (ImGui::MenuItem("Camera"))
										{
											IBaseComponent* component = gameobject->AddComponent(std::move(SceneComponentFactory::Create(SceneComponentType::CameraComponent, gameobject.get())));
											component->Initialise();

											/* fire a change of selection event to refresh the display */
											_EventService->Publish(Editor::Events::EventType::SelectedGameObjectChanged, std::make_unique<Events::SelectedGameObjectChangedArgs>(_SelectedGameObject));
										}

										ImGui::EndMenu();
									}
									ImGui::EndPopup();
								}

								/* Drag and Drop */
								if (ImGui::BeginDragDropSource())
								{
									ImGui::SetDragDropPayload("GAMEOBJECT_PAYLOAD", &gameobject->uid, sizeof(UID));
									ImGui::Text(gameobject->name.c_str());
									ImGui::EndDragDropSource();
								}

								rowIndexCnt++;
							}
						}
						else
						{
							ImGui::Text("No Scene Objects in Layer");
						}

						ImGui::EndChild();
					}
					else
					{
						ImGui::BeginChild("_Layer3DViewChild", ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowSize().y * .50f));

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

						ImGui::EndChild();
					}
				}
			}

			/* layer controls */
			bool layerControlsActive = (_SelectedSceneLayer != nullptr) && (_SelectedGameObject != nullptr);
			if (ImGui::TextButton("UP_GO", layerControlsActive && _SelectedGameObjectIdx > 0)) 
			{
				/* swap the current gameobject with the previous */
				std::iter_swap(_SelectedSceneLayer->GetGameObjects().begin() + _SelectedGameObjectIdx, _SelectedSceneLayer->GetGameObjects().begin() + _SelectedGameObjectIdx - 1);
				_SelectedGameObjectIdx--;
			}
			ImGui::SameLine();
			if (ImGui::TextButton("DOWN_GO", layerControlsActive && _SelectedGameObjectIdx < (_SelectedSceneLayer->GetGameObjects().size() - 1))) 
			{
				/* swap the current gameobject with the next */
				std::iter_swap(_SelectedSceneLayer->GetGameObjects().begin() + _SelectedGameObjectIdx, _SelectedSceneLayer->GetGameObjects().begin() + _SelectedGameObjectIdx + 1);
				_SelectedGameObjectIdx++;
			}
			ImGui::SameLine();
			if (ImGui::TextButton("DUP_GO", layerControlsActive)) {}
			ImGui::SameLine();
			if (ImGui::TextButton("DEL_GO", layerControlsActive)) 
			{
				_SelectedSceneLayer->RemoveChild(_SelectedGameObject->uid);
				_EventService->Publish(Editor::Events::EventType::SelectedGameObjectChanged, std::make_unique<Events::SelectedGameObjectChangedArgs>(nullptr));
			}
			ImGui::SameLine();
			if (ImGui::TextButton("ADD_GO", _SelectedSceneLayer != nullptr))
			{
				std::unique_ptr<GameObject> go = std::make_unique<GameObject>("Game Object");
				go->uid = UIDUtils::Create();

				go->transform = std::move(std::make_unique<Transform2DComponent>(&*go));

				/* create a new gameobject and add to current layer */
				_SelectedSceneLayer->AddGameObject(std::move(go));
			}
		}
	}

	void SpriteLayerEditor::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyReleasedEvent>(OSR_BIND_EVENT_FN(SpriteLayerEditor::OnKeyReleasedEvent));
	}


	void SpriteLayerEditor::OnSceneOpened(Events::EventArgs& args)
	{
		if (!_WorkspaceService->GetLoadedScene()->GetLayers().empty())
		{
			_SelectedSceneLayer = &*_WorkspaceService->GetLoadedScene()->GetLayers()[0];
			_SelectedSceneLayerIdx = 0;
		}
	}


	bool SpriteLayerEditor::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		return true;
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
		gameObject->GetGameObjects().size() == 0 ? TreeNodeEx_flags |= ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_None;			/* add children */
		(&*gameObject == _SelectedGameObject) ? TreeNodeEx_flags |= ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None;		/* currently selected */
		
		bool open = ImGui::TreeNodeEx(gameObject->name.c_str(), TreeNodeEx_flags);
				
		/* Sprite list context menu */
		if (ImGui::BeginPopupContextWindow() == true)
		{
			ImGui::Text("Context Menu: %s", gameObject->name.c_str());
			ImGui::EndPopup();
		}

		if (open)
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