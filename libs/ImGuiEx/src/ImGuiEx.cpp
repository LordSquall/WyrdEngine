/* local includes */
#include "ImGuiEx.h"

#include <core/ecs/Components.h>

using namespace Wyrd;

namespace ImGui 
{
    bool TreeEntity(Scene& scene, Entity selectedEntity, Entity e)
    {
        if (e != ENTITY_INVALID)
        {
            // retrieve the components for the current entity
            MetaDataComponent* metaDataComponent = scene.Get<MetaDataComponent>(e);
            RelationshipComponent* relationshipComponent = scene.Get<RelationshipComponent>(e);

            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;

            bool isSelected = selectedEntity == e;
            bool isParent = relationshipComponent->childrenCnt > 0;
            if (isParent)
            {
                bool isOpen = TreeNodeEx(metaDataComponent->name, flags);

                //if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
                //{
                //    _EventService->Publish(Editor::Events::EventType::SelectedEntityChanged, std::make_unique<Events::SelectedEntityChangedArgs>(entity));
                //}

                if (isOpen)
                {
                    Entity childEntity = relationshipComponent->first;
                    RelationshipComponent* nextRelationshipComponent = nullptr;
                    RelationshipComponent* lastRelationshipComponent = nullptr;
                    for (int i = 0; i < relationshipComponent->childrenCnt; i++)
                    {
                        if (childEntity != ENTITY_INVALID)
                        {
                            TreeEntity(scene, selectedEntity, childEntity);

                            nextRelationshipComponent = scene.Get<RelationshipComponent>(childEntity);
                            childEntity = nextRelationshipComponent->next;
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

                TreeNodeEx(metaDataComponent->name, flags);
            }
        }

        return false;
    }

    /* Hierarchy Objects */
    void TreeSeperator(int depth)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return;

        ImGuiContext& g = *GImGui;
        
        float thickness_draw = 4.0f;
        float thickness_layout = 1.0f;

        // Horizontal Separator
        float x1 = window->Pos.x;
        float x2 = window->Pos.x + window->Size.x;

        // FIXME-WORKRECT: old hack (#205) until we decide of consistent behavior with WorkRect/Indent and Separator
        if (g.GroupStack.Size > 0 && g.GroupStack.back().WindowID == window->ID)
            x1 += window->DC.Indent.x;

        // FIXME-WORKRECT: In theory we should simply be using WorkRect.Min.x/Max.x everywhere but it isn't aesthetically what we want,
        // need to introduce a variant of WorkRect for that purpose. (#4787)
        if (ImGuiTable* table = g.CurrentTable)
        {
            x1 = table->Columns[table->CurrentColumn].MinX;
            x2 = table->Columns[table->CurrentColumn].MaxX;
        }

        ImGuiOldColumns* columns = window->DC.CurrentColumns;
        if (columns)
            PushColumnsBackground();

        // We don't provide our width to the layout so that it doesn't get feed back into AutoFit
        // FIXME: This prevents ->CursorMaxPos based bounding box evaluation from working (e.g. TableEndCell)
        const ImRect bb(ImVec2(x1, window->DC.CursorPos.y), ImVec2(x2, window->DC.CursorPos.y + thickness_draw));
        ItemSize(ImVec2(0.0f, thickness_layout));
        const bool item_visible = ItemAdd(bb, 0);
        if (item_visible)
        {
            // Draw
            window->DrawList->AddLine({ bb.Min.x + (15 * depth), bb.Min.y}, ImVec2(bb.Max.x, bb.Min.y), GetColorU32(ImGuiCol_Separator));
            if (g.LogEnabled)
                LogRenderedText(&bb.Min, "--------------------------------\n");

        }
        if (columns)
        {
            PopColumnsBackground();
            columns->LineMinY = window->DC.CursorPos.y;
        }
    }
}