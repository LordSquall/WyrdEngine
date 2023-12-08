/* Local includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "core/ecs/ECS.h"
#include "core/ecs/Components.h"
#include "views/PropertiesViewer/PropGUIViews/PropGUIFactory.h"
#include "properties/BaseProp.h"
#include "core/Application.h"

#include <misc/cpp/imgui_stdlib.h>

namespace Wyrd
{
    void CreatePropGUI_Int(BaseProp* prop)
    {
        PropInt* intProp = (PropInt*)prop;
        ImGui::Text(prop->GetName().c_str());
        ImGui::SameLine();
        ImGui::PushID(prop->GetName().c_str());
        ImGui::InputInt("", (int*)&intProp->Value);
        ImGui::PopID();
    }

    void CreatePropGUI_Int32(BaseProp* prop)
    {
        PropInt32* int32Prop = (PropInt32*)prop;
        ImGui::Text(prop->GetName().c_str());
        ImGui::SameLine();
        ImGui::PushID(prop->GetName().c_str());
        ImGui::InputInt("", (int*)&int32Prop->Value);
        ImGui::PopID();
    }

    void CreatePropGUI_UInt32(BaseProp* prop)
    {
        PropUInt32* intU32Prop = (PropUInt32*)prop;
        ImGui::Text(prop->GetName().c_str());
        ImGui::SameLine();
        ImGui::PushID(prop->GetName().c_str());
        ImGui::InputInt("", (int*)&intU32Prop->Value);
        ImGui::PopID();
    }

    void CreatePropGUI_Int64(BaseProp* prop)
    {
        PropInt64* int64Prop = (PropInt64*)prop;
        ImGui::Text(prop->GetName().c_str());
        ImGui::SameLine();
        ImGui::PushID(prop->GetName().c_str());
        ImGui::InputInt("", (int*)&int64Prop->Value);
        ImGui::PopID();
    }

    void CreatePropGUI_UInt64(BaseProp* prop)
    {
        PropUInt64* intU64Prop = (PropUInt64*)prop;
        ImGui::Text(prop->GetName().c_str());
        ImGui::SameLine();
        ImGui::PushID(prop->GetName().c_str());
        ImGui::InputInt("", (int*)&intU64Prop->Value);
        ImGui::PopID();
    }

    void CreatePropGUI_Float(BaseProp* prop)
    {
        PropFloat* floatProp = (PropFloat*)prop;
        ImGui::Text(prop->GetName().c_str());
        ImGui::SameLine();
        ImGui::PushID(prop->GetName().c_str());
        ImGui::InputFloat("", (float*)&floatProp->Value);
        ImGui::PopID();
    }

    void CreatePropGUI_Double(BaseProp* prop)
    {
        PropDouble* doubleProp = (PropDouble*)prop;
        ImGui::Text(prop->GetName().c_str());
        ImGui::SameLine();
        ImGui::PushID(prop->GetName().c_str());
        ImGui::InputDouble("", (double*)&doubleProp->Value);
        ImGui::PopID();
    }

    void CreatePropGUI_String(BaseProp* prop)
    {
        PropDouble* doubleProp = (PropDouble*)prop;
        ImGui::Text(prop->GetName().c_str());
        ImGui::SameLine();
        ImGui::PushID(prop->GetName().c_str());
        ImGui::InputText("", (std::string*)&doubleProp->Value);
        ImGui::PopID();
    }

    void CreatePropGUI_Vec2(BaseProp* prop)
    {
        PropVec2* vec2Prop = (PropVec2*)prop;
        ImGui::Text(prop->GetName().c_str());
        ImGui::SameLine();
        ImGui::PushID(prop->GetName().c_str());
        ImGui::DragFloat2("", (float*)&vec2Prop->Value, 0.01f);
        ImGui::PopID();
    }

    void CreatePropGUI_Vec3(BaseProp* prop)
    {
        PropVec3* vec3Prop = (PropVec3*)prop;
        ImGui::Text(prop->GetName().c_str());
        ImGui::SameLine();
        ImGui::PushID(prop->GetName().c_str());
        ImGui::DragFloat3("", (float*)&vec3Prop->Value);
        ImGui::PopID();
    }

    void CreatePropGUI_Bool(BaseProp* prop)
    {
        PropBool* boolProp = (PropBool*)prop;
        ImGui::Text(prop->GetName().c_str());
        ImGui::SameLine();
        ImGui::PushID(prop->GetName().c_str());
        ImGui::Checkbox("", (bool*)&boolProp->Value);
        ImGui::PopID();
    }

    void CreatePropGUI_Color(BaseProp* prop)
    {
        PropColor* colorProp = (PropColor*)prop;
        ImGui::Text(prop->GetName().c_str());
        ImGui::SameLine();
        ImGui::PushID(prop->GetName().c_str());
        ImGui::ColorEdit4("", (float*)&colorProp->Value);
        ImGui::PopID();
    }

    void CreatePropGUI_Texture(BaseProp* prop)
    {
        PropTexture* textureProp = (PropTexture*)prop;
        ImGui::Text(prop->GetName().c_str());

        auto texture = textureProp->Value ? textureProp->Value : Application::Get().GetResources().Textures[UID(RES_TEXTURE_DEFAULT)].get();

        ImGui::Image((ImTextureID)(INT_PTR)texture->GetHandle(), ImVec2(64.0f, 64.0f));
        
        //if (ImGui::IsItemHovered())
        //{
        //    ImGui::BeginTooltip();
        //    ImGui::Image((ImTextureID)(INT_PTR)texture->GetHandle(), ImVec2(64.0f, 64.0f));
        //    ImGui::EndTooltip();
        //}
        

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_DND_TEXTURE))
            {
                UID* spriteUID = (UID*)payload->Data;

                /* retrieve the new texture */
                auto newTexture = Application::Get().GetResources().Textures[*spriteUID];

                if (newTexture)
                {
                    textureProp->Set<Texture*>(newTexture.get());
                }
                else
                {
                    WYRD_ERROR("Unable to find source Texture from Drag and Drop Payload!");
                }
            }
            ImGui::EndDragDropTarget();
        } 
        ImGui::SameLine();
        ImGui::Text(texture->GetUID().str().c_str());
    }

    void CreatePropGUI_Entity(BaseProp* prop)
    {
        PropEntity* entityProp = (PropEntity*)prop;
        Entity ent = entityProp->Value;

        ImGui::Text(prop->GetName().c_str());
        ImGui::SameLine();
        if (ent != ENTITY_INVALID)
        {
            ImGui::Text("%u", ent);
        }
        else
        {
            ImGui::Text("Unassigned");
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_DND_ENTITY))
            {
                Entity* entity = (Entity*)payload->Data;
                entityProp->Set<Entity>(*entity);
            }
            ImGui::EndDragDropTarget();
        }
    }

    std::map<std::string, PropGUIFactory::DrawPropGUIFunc>* PropGUIFactory::GetPropsGUI()
    {
        static std::map<std::string, PropGUIFactory::DrawPropGUIFunc> properties{
            { "Int", CreatePropGUI_Int },
            { "Int32", CreatePropGUI_Int32 },
            { "UInt32", CreatePropGUI_UInt32 },
            { "Int64", CreatePropGUI_Int64 },
            { "UInt64", CreatePropGUI_UInt64 },
            { "Float", CreatePropGUI_Float },
            { "Double", CreatePropGUI_Double },
            { "String", CreatePropGUI_String },
            { "Vec2", CreatePropGUI_Vec2 },
            { "Vec3", CreatePropGUI_Vec3 },
            { "Bool", CreatePropGUI_Bool },
            { "Color", CreatePropGUI_Color },
            { "Texture", CreatePropGUI_Texture },
            { "Entity", CreatePropGUI_Entity }
        };
        return &properties;
    }

    std::map<std::string, PropGUIFactory::DrawPropGUIFunc>* PropGUIFactory::GetManagedPropsGUI()
    {
        static std::map<std::string, PropGUIFactory::DrawPropGUIFunc> properties{
            { "System.Int", CreatePropGUI_Int },
            { "System.Int32", CreatePropGUI_Int32 },
            { "System.UInt32", CreatePropGUI_UInt32 },
            { "System.Int64", CreatePropGUI_Int64 },
            { "System.UInt64", CreatePropGUI_UInt64 },
            { "System.Single", CreatePropGUI_Float },
            { "System.Double", CreatePropGUI_Double },
            { "System.String", CreatePropGUI_String },
            { "WyrdAPI.Vector2", CreatePropGUI_Vec2 },
            { "WyrdAPI.Vector3", CreatePropGUI_Vec3 },
            { "System.Boolean", CreatePropGUI_Bool },
            { "WyrdAPI.Color", CreatePropGUI_Color },
            { "WyrdAPI.Texture", CreatePropGUI_Texture },
            { "WyrdAPI.Entity", CreatePropGUI_Entity }
        };
        return &properties;
    }

    bool PropGUIFactory::Register(const std::string typeName, DrawPropGUIFunc createFunc)
    {
        auto properties = GetPropsGUI();
        if (auto it = properties->find(typeName); it == properties->end())
        {
            (*properties)[typeName] = createFunc;
            return true;
        }
        return false;
    }

    void PropGUIFactory::DrawProp(BaseProp* prop)
    {
        auto properties = GetPropsGUI();
        if (properties->find(prop->GetType()) != properties->end())
        {
            properties->at(prop->GetType())(prop);
        }
        else
        {
            ImGui::TextColored({ 0.9f, 0.1f, 0.1f, 1.0f }, prop->GetName().c_str());
        }
    }

    void PropGUIFactory::DrawManagedProp(BaseProp* prop)
    {
        auto properties = GetManagedPropsGUI();
        if (properties->find(prop->GetManagedType()) != properties->end())
        {
            properties->at(prop->GetManagedType())(prop);
        }
        else
        {
            ImGui::TextColored({ 0.9f, 0.1f, 0.1f, 1.0f }, prop->GetName().c_str());
        }
    }

    void PropGUIFactory::DrawMissingProp(const std::string& name)
    {
        ImGui::Text("Missing GUI for prop %s", name.c_str());
    }
}
