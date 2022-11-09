/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Layer.h>
#include <core/KeyCodes.h>
#include <core/ecs/EntitySet.h>

/* local includes */
#include "HierarchyView.h"
#include "datamodels/EditorComponents.h"
#include "support/ImGuiUtils.h"
#include "support/RelationshipHelperFuncs.h"

namespace Wyrd::Editor
{
	HierarchyView::HierarchyView(EditorLayer* editorLayer) : EditorViewBase("Hierarchy", editorLayer), _SelectedEntity(ENTITY_INVALID) { }

	HierarchyView::~HierarchyView() { }

	void HierarchyView::OnInitialise()
	{
		_EventService = ServiceManager::Get<EventService>();
		_WorkspaceService = ServiceManager::Get<WorkspaceService>();
		_ResourceService = ServiceManager::Get<ResourceService>();

		_EventService->Subscribe(Events::EventType::SelectedEntityChanged, WYRD_BIND_FN(HierarchyView::OnSelectedEntityChanged));
	}

	void HierarchyView::OnEditorRender()
	{		
		if (_WorkspaceService->IsSceneLoaded())
		{
			Scene& scene = *_WorkspaceService->GetLoadedScene();

			const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;

			static ImGuiTableFlags flags = ImGuiTableFlags_NoBordersInBody;

			if (ImGui::BeginTable("entityHierarchyTable", 1, flags))
			{
				// The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
				ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
				ImGui::TableHeadersRow();
			
				for (Entity e : EntitySet<RelationshipComponent>(scene))
				{
					RelationshipComponent* relationshipComponent = scene.Get<RelationshipComponent>(e);
			
					if (relationshipComponent->parent == ENTITY_INVALID)
					{
						DisplayEntity(e);
					}
				}
				ImGui::EndTable();
			}
			
			/*
			* this will create a selectable area on the reset of the available area which is invisible on the display
			* using this will allow the context menu to function when nothing is selected in the table
			*/
			ImGui::Selectable("##label", false, ImGuiSelectableFlags_Disabled, ImGui::GetContentRegionAvail());
			DisplayEntityContentMenu(ENTITY_INVALID);
			
			for (Entity e : EntitySet<RelationshipComponent>(scene))
			{
				RelationshipComponent* relationshipComponent = scene.Get<RelationshipComponent>(e);
			
				if (relationshipComponent->remove == true)
				{
					scene.DestroyEntity(e);
					relationshipComponent->remove = false;
				}
			}
		}
	}


	void HierarchyView::DisplayEntity(Entity entity)
	{
		Scene& scene = *_WorkspaceService->GetLoadedScene();
		
		MetaDataComponent* metaDataComponent = scene.Get<MetaDataComponent>(entity);
		RelationshipComponent* relationshipComponent = scene.Get<RelationshipComponent>(entity);
		
		bool selected = entity == _SelectedEntity;
		
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth;
		
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::PushID(entity);
		const bool isParent = (relationshipComponent->childrenCnt > 0);
		if (isParent)
		{
			flags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
			flags |= (selected) ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None;
			bool open = ImGui::TreeNodeEx(metaDataComponent->name, flags);
		
			DisplayEntityContentMenu(entity);
			DisplayEntityDragAndDrop(entity);
			DisplayEntityTooltip(entity);
		
			if (ImGui::IsItemHovered() && ImGui::IsItemClicked(ImGuiMouseButton_Left))
			{
				_EventService->Publish(Editor::Events::EventType::SelectedEntityChanged, std::make_unique<Events::SelectedEntityChangedArgs>(entity));
			}
		
			if (open)
			{
				Entity childEntity = relationshipComponent->first;
		
				for (int i = 0; i < relationshipComponent->childrenCnt; i++)
				{
					if (childEntity != ENTITY_INVALID)
					{
						DisplayEntity(childEntity);
		
						//RelationshipComponent* nextRelationshipComponent = scene.Get<RelationshipComponent>(childEntity);
						//childEntity = nextRelationshipComponent->next;
					}
				}
		
				ImGui::TreePop();
			}
		}
		else
		{
			flags |= ImGuiTreeNodeFlags_Leaf;
			flags |= ImGuiTreeNodeFlags_Bullet;
			flags |= ImGuiTreeNodeFlags_NoTreePushOnOpen;
			flags |= (selected) ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None;
			ImGui::TreeNodeEx(metaDataComponent->name, flags);
		
			DisplayEntityContentMenu(entity);
			DisplayEntityDragAndDrop(entity);
			DisplayEntityTooltip(entity);
		
			if (ImGui::IsItemHovered() && ImGui::IsItemClicked(ImGuiMouseButton_Left))
			{
				_EventService->Publish(Editor::Events::EventType::SelectedEntityChanged, std::make_unique<Events::SelectedEntityChangedArgs>(entity));
			}
		}
		ImGui::PopID();
	}

	void HierarchyView::DisplayEntityContentMenu(Entity entity)
	{
		if (ImGui::BeginPopupContextItem((const char*)&entity))
		{
			Scene& scene = *_WorkspaceService->GetLoadedScene();
			MetaDataComponent* metaDataComponent = scene.Get<MetaDataComponent>(entity);
			
			if (ImGui::MenuItem("Add Child"))
			{
				/* build the base default entity */
				Entity newEntity = scene.CreateEntity();
				scene.AssignComponent<MetaDataComponent>(newEntity);
				scene.AssignComponent<Transform2DComponent>(newEntity);
				scene.AssignComponent<Editor::EditorComponent>(newEntity);
				scene.AssignComponent<RelationshipComponent>(newEntity);
			
				if (entity != ENTITY_INVALID)
				{
					RelationshipHelperFuncs::AddChild(&scene, entity, newEntity);
				}
				
			}
			if (ImGui::MenuItem("Delete"))
			{
				/* update the relationship component */
				_EventService->Publish(Editor::Events::EventType::SelectedEntityChanged, std::make_unique<Events::SelectedEntityChangedArgs>(ENTITY_INVALID));
				RelationshipHelperFuncs::Remove(&scene, entity);
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Add Transform"))
			{
				Transform2DComponent* c = scene.AssignComponent<Transform2DComponent>(entity);
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Add Sprite"))
			{
				SpriteComponent* c = scene.AssignComponent<SpriteComponent>(entity);
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Add Script"))
			{
				ScriptComponent* c = scene.AssignComponent<ScriptComponent>(entity);
			}
			//ImGui::Separator();
			//if (ImGui::MenuItem("Add Text"))
			//{
			//	TextComponent* c = scene.AssignComponent<TextComponent>(entity);
			//}
			if (ImGui::MenuItem("Add Camera"))
			{
				CameraComponent* c = scene.AssignComponent<CameraComponent>(entity);
			}
			ImGui::EndPopup();
		}
	}

	void HierarchyView::DisplayEntityDragAndDrop(Entity entity)
	{
		Scene& scene = *_WorkspaceService->GetLoadedScene();
		MetaDataComponent* metaDataComponent = scene.Get<MetaDataComponent>(entity);

		/* Drag and Drop */
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload(IMGUI_DND_ENTITY, &entity, sizeof(Entity));
			ImGui::Text(metaDataComponent->name);
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_DND_ENTITY))
			{
				Entity* sourceEntity = (Entity*)payload->Data;

				RelationshipComponent* source = scene.Get<RelationshipComponent>(*sourceEntity);
				RelationshipComponent* target = scene.Get<RelationshipComponent>(entity);
				
				/* 
				* this is a complex operation, firstly we need to remove the source relationship
				* before building the new relationship
				*/
				if (source->parent != ENTITY_INVALID)
				{
					/* retrieve surrounding nodes */
					RelationshipComponent* parent = scene.Get<RelationshipComponent>(source->parent);
				
					/* close the double linked list */
					if (source->previous == ENTITY_INVALID && source->next == ENTITY_INVALID)
					{
						/* only child */
					}
					else if (source->previous == ENTITY_INVALID)
					{
						/* first child */
						RelationshipComponent* next = scene.Get<RelationshipComponent>(source->next);
				
						parent->first = source->next;
						next->previous = ENTITY_INVALID;
					}
					else if (source->next == ENTITY_INVALID)
					{
						/* last child*/
						RelationshipComponent* prev = scene.Get<RelationshipComponent>(source->previous);
				
						prev->next = ENTITY_INVALID;
					}
					else
					{
						/* middle child */
						RelationshipComponent* next = scene.Get<RelationshipComponent>(source->next);
						RelationshipComponent* prev = scene.Get<RelationshipComponent>(source->previous);
						next->previous = source->previous;
						prev->next = source->next;
					}
				
					/* decrement the parent counter */
					parent->childrenCnt -= 1;
				}
				
				/* retrieve surrounding nodes */
				RelationshipComponent* first = scene.Get<RelationshipComponent>(target->first);
				RelationshipComponent* next = scene.Get<RelationshipComponent>(target->next);
				RelationshipComponent* prev = scene.Get<RelationshipComponent>(target->previous);
				
				/* insert to the start of the linked list */
				source->parent = entity;
				source->next = target->first;
				
				if (first != nullptr)
				{
					first->previous = *sourceEntity;
				}
				
				target->first = *sourceEntity;
				
				/* increment parent counter */
				target->childrenCnt += 1;

			}
			ImGui::EndDragDropTarget();
		}
	}

	void HierarchyView::DisplayEntityTooltip(Entity entity)
	{
		Scene& scene = *_WorkspaceService->GetLoadedScene();

		if (ImGui::IsItemHovered())
		{
			//MetaDataComponent* metadataComp = scene.Get<MetaDataComponent>(entity);
			//RelationshipComponent* relationshipComp = scene.Get<RelationshipComponent>(entity);
			//
			//ImGui::BeginTooltip();
			//ImGui::Text("Entity: %lu", entity);
			//ImGui::Text("Name: %s", metadataComp->name);
			//ImGui::Text("childCnt: %d", relationshipComp->childrenCnt);
			//ImGui::Text("depth: %d", relationshipComp->depth);
			//ImGui::Text("first: %lu", relationshipComp->first);
			//ImGui::Text("next: %lu", relationshipComp->next);
			//ImGui::Text("parent: %lu", relationshipComp->parent);
			//ImGui::Text("previous: %lu", relationshipComp->previous);
			//ImGui::EndTooltip();
		}
	}

	void HierarchyView::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(WYRD_BIND_EVENT_FN(HierarchyView::OnKeyPressedEvent), nullptr);
	}

	bool HierarchyView::OnKeyPressedEvent(KeyPressedEvent& e, void* data)
	{
		if (e.GetKeyCode() == OSR_KEY_F2)
		{
			WYRD_CORE_TRACE("Edit Entity Name");
		}
		return true;
	}

	void HierarchyView::OnSelectedEntityChanged(Events::EventArgs& args)
	{
		Events::SelectedEntityChangedArgs& evtArgs = static_cast<Events::SelectedEntityChangedArgs&>(args);

		_SelectedEntity = evtArgs.entity;
	}
}