#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Layer.h>
#include <core/KeyCodes.h>
#include <core/ecs/EntitySet.h>

/* local includes */
#include "SceneEditor.h"
#include "support/ImGuiUtils.h"

namespace Wyrd::Editor
{
	SceneEditor::SceneEditor(EditorLayer* editorLayer) : EditorViewBase("Scene Editor", editorLayer), _SelectedEntity(ENTITY_INVALID) { }

	SceneEditor::~SceneEditor() { }

	void SceneEditor::OnInitialise()
	{
		_EventService = ServiceManager::Get<EventService>(ServiceManager::Service::Events);
		_WorkspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Service::Workspace);
		_ResourceService = ServiceManager::Get<ResourceService>(ServiceManager::Service::Resources);

		_EventService->Subscribe(Events::EventType::SelectedEntityChanged, EVENT_FUNC(SceneEditor::OnSelectedEntityChanged));
	}

	void SceneEditor::OnEditorRender()
	{		
		if (_WorkspaceService->IsSceneLoaded())
		{
			Scene& scene = *_WorkspaceService->GetLoadedScene();
		
			if (ImGui::CollapsingHeader("Entities", ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (ImGui::BeginTable("spriteLayerTable2", 1, ImGuiTableFlags_SizingFixedFit))
				{
					ImGui::TableSetupColumn("Name");

					int rowIndexCnt = 0;

					for (Entity e : EntitySet<MetaDataComponent>(scene))
					{
						MetaDataComponent* metadataComp = scene.Get<MetaDataComponent>(e);

						ImGui::PushID(e);
						ImGui::TableNextRow(ImGuiTableRowFlags_None, ImGui::GetTextLineHeightWithSpacing() * 1.2f);
						ImGui::TableNextColumn();

						bool selected = e == _SelectedEntity;
						if (ImGui::Selectable(metadataComp->name, &selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap))
						{
							WYRD_TRACE("Selected: {0}", metadataComp->name);
							_EventService->Publish(Editor::Events::EventType::SelectedEntityChanged, std::make_unique<Events::SelectedEntityChangedArgs>(e));
							_SelectedEntity = e;
						}

						/* Drag and Drop */
						if (ImGui::BeginDragDropSource())
						{
							ImGui::SetDragDropPayload("DND_ENTITY", &e, sizeof(Entity));
							ImGui::Text(metadataComp->name);
							ImGui::EndDragDropSource();
						}

						if (ImGui::IsItemHovered())
						{
							// Tooltip
						}

						if (ImGui::BeginPopupContextItem())
						{
							if (ImGui::MenuItem("Add Transform"))
							{
								Transform2DComponent* c = scene.AssignComponent<Transform2DComponent>(e);
								c->position = { 0.0f, 0.0f };
								c->rotation = 0.0f;
							}
							ImGui::Separator();
							if (ImGui::MenuItem("Add Sprite"))
							{
								SpriteComponent* c = scene.AssignComponent<SpriteComponent>(e);
								c->position = { 0.0f, 0.0f };
								c->size = { 32.0f, 32.0f };
								c->texture = UID(RESOURCE_DEFAULT_TEXTURE);
								c->color = { 1.0f, 1.0f, 1.0f, 1.0f };
							}
							ImGui::Separator();
							if (ImGui::MenuItem("Add Text"))
							{
								TextComponent* c = scene.AssignComponent<TextComponent>(e);
								sprintf(c->content, "Text");
							}
							ImGui::Separator();
							if (ImGui::MenuItem("Add Script"))
							{
								ScriptComponent* c = scene.AssignComponent<ScriptComponent>(e);
								c->script = UID();
							}
							if (ImGui::MenuItem("Add Camera"))
							{
								CameraComponent* c = scene.AssignComponent<CameraComponent>(e);
								c->size = 10.0f;
							}
							ImGui::EndPopup();
						}

						ImGui::PopID();

						rowIndexCnt++;
					}

					ImGui::EndTable();
				}

				/* Entity controls */
				if (ImGui::TextButton("^", (_SelectedEntity != ENTITY_INVALID) && (_SelectedEntity - 1 != ENTITY_INVALID)))
				{
					scene.SwapEntity(_SelectedEntity, _SelectedEntity - 1);
					_EventService->Publish(Editor::Events::EventType::SelectedEntityChanged, std::make_unique<Events::SelectedEntityChangedArgs>(_SelectedEntity - 1));
				}
				ImGui::SameLine();
				if (ImGui::TextButton("v", (_SelectedEntity != ENTITY_INVALID) && (_SelectedEntity + 1 != (scene.entities.size()+1))))
				{
					scene.SwapEntity(_SelectedEntity, _SelectedEntity + 1);
					_EventService->Publish(Editor::Events::EventType::SelectedEntityChanged, std::make_unique<Events::SelectedEntityChangedArgs>(_SelectedEntity + 1));
				}
				ImGui::SameLine();
				if (ImGui::TextButton("R")) 
				{
					scene.DestroyEntity(_SelectedEntity);
					_EventService->Publish(Editor::Events::EventType::SelectedEntityChanged, std::make_unique<Events::SelectedEntityChangedArgs>(ENTITY_INVALID));
				}
				ImGui::SameLine();
				if (ImGui::TextButton("A")) 
				{
					Entity e = scene.CreateEntity();
					MetaDataComponent* dataComponent = scene.AssignComponent<MetaDataComponent>(e);
					strcpy(dataComponent->name, "New Entity");

					Transform2DComponent* transformComponent = scene.AssignComponent<Transform2DComponent>(e);
					SpriteComponent* cs = scene.AssignComponent<SpriteComponent>(e);
					cs->color = { 1.0f, 1.0f, 1.0f, 1.0f };
					cs->size = { 32.0f, 32.0f };
					cs->texture = UID(RESOURCE_DEFAULT_TEXTURE);

					//ECSEditorComponent* editorComponent = scene.AssignComponent<ECSEditorComponent>(e);
				}
			}
		}
	}


	void SceneEditor::OnSelectedEntityChanged(Events::EventArgs& args)
	{
		Events::SelectedEntityChangedArgs& evtArgs = static_cast<Events::SelectedEntityChangedArgs&>(args);

		_SelectedEntity = evtArgs.entity;
	}
}