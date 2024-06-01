/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Layer.h>
#include <core/KeyCodes.h>
#include <core/ecs/EntitySet.h>
#include <core/ecs/HierarchyEntitySet.h>
#include <core/support/MaterialHelperFuncs.h>

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
		// We need to display the context menu first to preserve imgui id ordering
		DisplayContentMenu();

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
					RelationshipComponent* relationshipComp = scene.Get<RelationshipComponent>(e);

					if (relationshipComp->parent == ENTITY_INVALID)
					{
						DisplayEntity(e, 0);
					}
				}
				//DisplayBelowDragTarget(ENTITY_INVALID, 0);
				ImGui::EndTable();
			}
			
			/*
			* this will create a selectable area on the reset of the available area which is invisible on the display
			* using this will allow the context menu to function when nothing is selected in the table
			*/
			//ImGui::Selectable("##label", false, ImGuiSelectableFlags_Disabled, ImGui::GetContentRegionAvail());
			//DisplayEntityContentMenu(ENTITY_INVALID);
		}
	}


	void HierarchyView::DisplayEntity(Entity entity, int depth)
	{
		Scene& scene = *_WorkspaceService->GetLoadedScene();
		
		MetaDataComponent* metaDataComponent = scene.Get<MetaDataComponent>(entity);
		RelationshipComponent* relationshipComponent = scene.Get<RelationshipComponent>(entity);
		
		bool selected = entity == _SelectedEntity;
		
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
		
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::PushID(entity);

		const bool isParent = (relationshipComponent->childrenCnt > 0);

		if (isParent)
		{
			flags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
			flags |= ImGuiTreeNodeFlags_DefaultOpen;
		}
		else
		{
			flags |= ImGuiTreeNodeFlags_Leaf;
			flags |= ImGuiTreeNodeFlags_Bullet;
			flags |= ImGuiTreeNodeFlags_NoTreePushOnOpen;
		}
		flags |= (selected) ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None;

		std::string name(metaDataComponent->name);
		name.append(" - " + std::to_string(entity));
		bool open = ImGui::TreeNodeEx(name.c_str(), flags);

		if (open)
		{
			Entity nextEntity = relationshipComponent->next;
			RelationshipComponent* nextRelationshipComponent = nullptr;
			for (int i = 0; i < relationshipComponent->childrenCnt; i++)
			{
				if (nextEntity != ENTITY_INVALID)
				{
					//DisplayEntity(nextEntity, depth + 1);
				}
			
				nextEntity = scene.Get<RelationshipComponent>(nextEntity)->next;
			}
			//ImGui::TreePop();
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
				Entity newEntity = scene.CreateEntity("New Entity");
				scene.AssignComponent<Editor::EditorComponent>(newEntity);
				scene.AssignComponent<MeshRendererComponent>(newEntity);
				scene.AssignComponent<MaterialComponent>(newEntity);

				if (entity != ENTITY_INVALID)
				{
					RelationshipHelperFuncs::AddChild(&scene, newEntity, entity, RelationshipHelperFuncs::AddOp::On);
				}

				_EventService->Publish(Editor::Events::EventType::SelectedEntityChanged, std::make_unique<Events::SelectedEntityChangedArgs>(newEntity));
				
			}
			if (ImGui::MenuItem("Delete"))
			{
				/* update the relationship component */
				_EventService->Publish(Editor::Events::EventType::SelectedEntityChanged, std::make_unique<Events::SelectedEntityChangedArgs>(ENTITY_INVALID));
				RelationshipHelperFuncs::Remove(&scene, entity);
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Add Component"))
			{
				ImGui::MenuItem("test1");
				ImGui::MenuItem("test2");
			}
			if (ImGui::MenuItem("Add Transform"))
			{
				Transform2DComponent* c = scene.AssignComponent<Transform2DComponent>(entity);
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Add Mesh"))
			{
				scene.AssignComponent<Transform3DComponent>(entity);
				scene.AssignComponent<MeshRendererComponent>(entity);
			}
			if (ImGui::MenuItem("Add Sprite"))
			{
				SpriteComponent* c = scene.AssignComponent<SpriteComponent>(entity);
			}
			if (ImGui::MenuItem("Add Material"))
			{
				MaterialComponent* c = scene.AssignComponent<MaterialComponent>(entity);

				/* Retrieve the material */
				std::shared_ptr<Material> material = Application::Get().GetResources().Materials[RES_MATERIAL_3D_DEFAULT];

				MaterialInputMap propList = material->GetInputPropertyList();

				/* Clear out and recreate the property set */
				c->properties = std::make_shared<std::map<std::string, BasePropRef>>();

				/* Process each of the properties in the material and assign the data for the material component */
				for (auto& [name, binding] : propList)
				{
					(*c->properties)[name] = PropFactory::CreateProp(binding.type, name);
					(*c->properties)[name]->Deserialise(binding.defaultData);
				}
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Add Script"))
			{
				ScriptComponent* c = scene.AssignComponent<ScriptComponent>(entity);
			}
			if (ImGui::MenuItem("Add Camera"))
			{
				CameraComponent* c = scene.AssignComponent<CameraComponent>(entity);
			}
			ImGui::EndPopup();
		}
	}

	void HierarchyView::DisplayContentMenu()
	{
		if (ImGui::BeginPopupContextWindow())
		{
			Scene& scene = *_WorkspaceService->GetLoadedScene();
			
			if (ImGui::MenuItem("Add Child"))
			{
				Entity newEntity = scene.CreateEntity("New Entity");
				EditorComponent* ec = scene.AssignComponent<EditorComponent>(newEntity);
				MeshRendererComponent* mrc = scene.AssignComponent<MeshRendererComponent>(newEntity);
				MaterialComponent* mc = scene.AssignComponent<MaterialComponent>(newEntity);

				MaterialHelperFuncs::AssignToComponent(mc);

				_EventService->Publish(Editor::Events::EventType::SelectedEntityChanged, std::make_unique<Events::SelectedEntityChangedArgs>(newEntity));
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

		if (ImGui::GetDragDropPayload() != nullptr)
		{
			if (strcmp(ImGui::GetDragDropPayload()->DataType, IMGUI_DND_ENTITY) == 0)
			{
				Entity* payloadEntity = (Entity*)ImGui::GetDragDropPayload()->Data;

				if (RelationshipHelperFuncs::CanAddChild(&scene, entity, *payloadEntity, RelationshipHelperFuncs::AddOp::On))
				{
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_DND_ENTITY))
						{
							Entity* sourceEntity = (Entity*)payload->Data;

							RelationshipComponent* child = scene.Get<RelationshipComponent>(*sourceEntity);

							RelationshipHelperFuncs::AddChild(&scene, *sourceEntity, entity, RelationshipHelperFuncs::AddOp::On);
						}
						ImGui::EndDragDropTarget();
					}
				}
			}
		}
	}

	void HierarchyView::DisplayEntityTooltip(Entity entity)
	{
		Scene& scene = *_WorkspaceService->GetLoadedScene();

		if (ImGui::IsItemHovered())
		{
			MetaDataComponent* metadataComp = scene.Get<MetaDataComponent>(entity);
			RelationshipComponent* relationshipComp = scene.Get<RelationshipComponent>(entity);
			
			ImGui::BeginTooltip();
			ImGui::Text("Entity: %lu", entity);
			ImGui::Text("Name: %s", metadataComp->name);
			ImGui::Text("childCnt: %d", relationshipComp->childrenCnt);
			ImGui::Text("depth: %d", relationshipComp->depth);
			ImGui::Text("first: %lu", relationshipComp->first);
			ImGui::Text("last: %lu", relationshipComp->last);
			ImGui::Text("next: %lu", relationshipComp->next);
			ImGui::Text("parent: %lu", relationshipComp->parent);
			ImGui::Text("previous: %lu", relationshipComp->previous);
			ImGui::EndTooltip();
		}
	}

	void HierarchyView::DisplayAboveDragTarget(Entity entity, int depth)
	{
		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;

		ImGui::TreeSeperator(depth);
		
		Scene& scene = *_WorkspaceService->GetLoadedScene();

		if (ImGui::GetDragDropPayload() != nullptr)
		{
			if (strcmp(ImGui::GetDragDropPayload()->DataType, IMGUI_DND_ENTITY) == 0)
			{
				Entity* payloadEntity = (Entity*)ImGui::GetDragDropPayload()->Data;

				if (RelationshipHelperFuncs::CanAddChild(&scene, entity, *payloadEntity, RelationshipHelperFuncs::AddOp::Before))
				{
					if (ImGui::BeginDragDropTarget())
					{

						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_DND_ENTITY))
						{
							Entity* sourceEntity = (Entity*)payload->Data;

							RelationshipComponent* child = scene.Get<RelationshipComponent>(*sourceEntity);

							RelationshipHelperFuncs::AddChild(&scene, *sourceEntity, entity, RelationshipHelperFuncs::AddOp::Before);
						}
						ImGui::EndDragDropTarget();
					}
				}
			}
		}
	}

	void HierarchyView::DisplayBelowDragTarget(Entity entity, int depth)
	{
		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;

		ImGui::TreeSeperator(depth);

		Scene& scene = *_WorkspaceService->GetLoadedScene();
		if (ImGui::GetDragDropPayload() != nullptr)
		{
			if (strcmp(ImGui::GetDragDropPayload()->DataType, IMGUI_DND_ENTITY) == 0)
			{
				Entity* payloadEntity = (Entity*)ImGui::GetDragDropPayload()->Data;

				if (RelationshipHelperFuncs::CanAddChild(&scene, entity, *payloadEntity, RelationshipHelperFuncs::AddOp::After))
				{
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_DND_ENTITY))
						{
							Entity* sourceEntity = (Entity*)payload->Data;

							RelationshipComponent* child = scene.Get<RelationshipComponent>(*sourceEntity);

							RelationshipHelperFuncs::AddChild(&scene, *sourceEntity, entity, RelationshipHelperFuncs::AddOp::After);
						}
						ImGui::EndDragDropTarget();
					}
				}
			}
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