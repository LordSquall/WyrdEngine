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

			// Generate Entity Hierachy Data structure
			std::vector<EntityHiearchyNode*> nodes;
			std::stack<EntityHiearchyNode*> nodeStack;

			for (Entity e : HierarchyEntitySet<RelationshipComponent>(scene))
			{
				RelationshipComponent* rc = scene.Get<RelationshipComponent>(e);
				RelationshipComponent* topRc = nodeStack.empty() ? nullptr : scene.Get<RelationshipComponent>(nodeStack.top()->e);

				if (rc->depth == 0)
				{
					EntityHiearchyNode* ehn = new EntityHiearchyNode();
					ehn->e = e;
					nodes.push_back(ehn);
					nodeStack.push(ehn);

				}
				else if (rc->depth > topRc->depth)
				{
					EntityHiearchyNode* ehn = new EntityHiearchyNode();
					ehn->e = e;
					nodeStack.top()->children.push_back(ehn);
					nodeStack.push(ehn);
				}
			}
		
			const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
		
			static ImGuiTableFlags flags = ImGuiTableFlags_NoBordersInBody;
		
			if (ImGui::BeginTable("entityHierarchyTable", 1, flags))
			{
				ImGui::TableSetupColumn("Name");
				ImGui::TableHeadersRow();
			
				for (const EntityHiearchyNode* node : nodes)
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();

					ImGui::PushID((int)node->e);
					DisplayEntityNode(*node);
					ImGui::PopID();
				}
				ImGui::EndTable();
			}

			for(EntityHiearchyNode* e : nodes)
			{
				delete e;
			}
		}
	}


	void HierarchyView::DisplayEntityNode(const EntityHiearchyNode& entityNode)
	{
		Scene& scene = *_WorkspaceService->GetLoadedScene();
		
		MetaDataComponent* metaDataComponent = scene.Get<MetaDataComponent>(entityNode.e);
		RelationshipComponent* relationshipComponent = scene.Get<RelationshipComponent>(entityNode.e);
		
		bool selected = entityNode.e == _SelectedEntity;
		
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
		
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
		}
		flags |= (selected) ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None;

		std::string name(metaDataComponent->name);
		name.append(" - " + std::to_string(entityNode.e));
		bool open = ImGui::TreeNodeEx(name.c_str(), flags);

		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			_EventService->Publish(Editor::Events::EventType::SelectedEntityChanged, std::make_unique<Events::SelectedEntityChangedArgs>(entityNode.e));
		}

		DisplayEntityNodeContentMenu(entityNode);

		if (open)
		{
			for (const EntityHiearchyNode* node : entityNode.children)
			{
				ImGui::PushID((int)node->e);
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
			
				DisplayEntityNode(*node);
			
				ImGui::PopID();
			}

			ImGui::TreePop();
		}
	}

	void HierarchyView::DisplayEntityNodeContentMenu(const EntityHiearchyNode& entityNode)
	{
		if (ImGui::BeginPopupContextItem((const char*)&entityNode.e))
		{
			Scene& scene = *_WorkspaceService->GetLoadedScene();
			MetaDataComponent* metaDataComponent = scene.Get<MetaDataComponent>(entityNode.e);
			ImGui::Text("%d", entityNode.e);

			if (ImGui::MenuItem("Add Child"))
			{
				/* build the base default entity */
				Entity newEntity = scene.CreateEntity("New Entity");
				scene.AssignComponent<Editor::EditorComponent>(newEntity);
				scene.AssignComponent<MeshRendererComponent>(newEntity);
				scene.AssignComponent<MaterialComponent>(newEntity);

				if (entityNode.e != ENTITY_INVALID)
				{
					RelationshipHelperFuncs::AddChild(&scene, newEntity, entityNode.e, RelationshipHelperFuncs::AddOp::On);
				}

				_EventService->Publish(Editor::Events::EventType::SelectedEntityChanged, std::make_unique<Events::SelectedEntityChangedArgs>(newEntity));
				
			}
			if (ImGui::MenuItem("Delete"))
			{
				/* update the relationship component */
				_EventService->Publish(Editor::Events::EventType::SelectedEntityChanged, std::make_unique<Events::SelectedEntityChangedArgs>(ENTITY_INVALID));
				RelationshipHelperFuncs::Remove(&scene, entityNode.e);
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Add Component"))
			{
				ImGui::MenuItem("test1");
				ImGui::MenuItem("test2");
			}
			if (ImGui::MenuItem("Add Transform"))
			{
				Transform2DComponent* c = scene.AssignComponent<Transform2DComponent>(entityNode.e);
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Add Mesh"))
			{
				scene.AssignComponent<Transform3DComponent>(entityNode.e);
				scene.AssignComponent<MeshRendererComponent>(entityNode.e);
			}
			if (ImGui::MenuItem("Add Sprite"))
			{
				SpriteComponent* c = scene.AssignComponent<SpriteComponent>(entityNode.e);
			}
			if (ImGui::MenuItem("Add Material"))
			{
				MaterialComponent* c = scene.AssignComponent<MaterialComponent>(entityNode.e);

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
				ScriptComponent* c = scene.AssignComponent<ScriptComponent>(entityNode.e);
			}
			if (ImGui::MenuItem("Add Camera"))
			{
				CameraComponent* c = scene.AssignComponent<CameraComponent>(entityNode.e);
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
				/* Ensure that the new Entity is placed at the end of the Hierarchy change */
				Entity lastEntity = RelationshipHelperFuncs::GetLast(&scene);

				/* Create a new entity within the scene */
				Entity newEntity = scene.CreateEntity("New Entity");
				EditorComponent* ec = scene.AssignComponent<EditorComponent>(newEntity);
				MeshRendererComponent* mrc = scene.AssignComponent<MeshRendererComponent>(newEntity);
				MaterialComponent* mc = scene.AssignComponent<MaterialComponent>(newEntity);

				MaterialHelperFuncs::AssignToComponent(mc);
				
				RelationshipComponent* currentRC = scene.Get< RelationshipComponent>(newEntity);
				RelationshipHelperFuncs::AddChild(&scene, ENTITY_INVALID, newEntity, RelationshipHelperFuncs::AddOp::On);
				
				if (lastEntity != ENTITY_INVALID)
				{
					RelationshipComponent* lastRC = scene.Get<RelationshipComponent>(lastEntity);

					lastRC->next = newEntity;
				}

				/* Publish the change to ensure we are selecting the new entity on creation */
				_EventService->Publish(Editor::Events::EventType::SelectedEntityChanged, std::make_unique<Events::SelectedEntityChangedArgs>(newEntity));
			}
			ImGui::EndPopup();
		}
	}

	void HierarchyView::DisplayEntityNodeDragAndDrop(const EntityHiearchyNode& entityNode)
	{
		Scene& scene = *_WorkspaceService->GetLoadedScene();
		MetaDataComponent* metaDataComponent = scene.Get<MetaDataComponent>(entityNode.e);

		/* Drag and Drop */
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload(IMGUI_DND_ENTITY, &entityNode.e, sizeof(Entity));
			ImGui::Text(metaDataComponent->name);
			ImGui::EndDragDropSource();
		}

		if (ImGui::GetDragDropPayload() != nullptr)
		{
			if (strcmp(ImGui::GetDragDropPayload()->DataType, IMGUI_DND_ENTITY) == 0)
			{
				Entity* payloadEntity = (Entity*)ImGui::GetDragDropPayload()->Data;

				if (RelationshipHelperFuncs::CanAddChild(&scene, entityNode.e, *payloadEntity, RelationshipHelperFuncs::AddOp::On))
				{
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_DND_ENTITY))
						{
							Entity* sourceEntity = (Entity*)payload->Data;

							RelationshipComponent* child = scene.Get<RelationshipComponent>(*sourceEntity);

							RelationshipHelperFuncs::AddChild(&scene, *sourceEntity, entityNode.e, RelationshipHelperFuncs::AddOp::On);
						}
						ImGui::EndDragDropTarget();
					}
				}
			}
		}
	}

	void HierarchyView::DisplayEntityNodeTooltip(const EntityHiearchyNode& entityNode)
	{
		Scene& scene = *_WorkspaceService->GetLoadedScene();

		if (ImGui::IsItemHovered())
		{
			MetaDataComponent* metadataComp = scene.Get<MetaDataComponent>(entityNode.e);
			RelationshipComponent* relationshipComp = scene.Get<RelationshipComponent>(entityNode.e);
			
			ImGui::BeginTooltip();
			ImGui::Text("Entity: %lu", entityNode.e);
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