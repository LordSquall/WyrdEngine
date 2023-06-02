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
        ImGui::InputInt(prop->GetName().c_str(), (int*)&intProp->Value);
    }

    void CreatePropGUI_Int32(BaseProp* prop)
    {
        PropInt32* int32Prop = (PropInt32*)prop;
        ImGui::InputInt(prop->GetName().c_str(), (int*)&int32Prop->Value);
    }

    void CreatePropGUI_UInt32(BaseProp* prop)
    {
        PropUInt32* intU32Prop = (PropUInt32*)prop;
        ImGui::InputInt(prop->GetName().c_str(), (int*)&intU32Prop->Value);
    }

    void CreatePropGUI_Int64(BaseProp* prop)
    {
        PropInt64* int64Prop = (PropInt64*)prop;
        ImGui::InputInt(prop->GetName().c_str(), (int*)&int64Prop->Value);
    }

    void CreatePropGUI_UInt64(BaseProp* prop)
    {
        PropUInt64* intU64Prop = (PropUInt64*)prop;
        ImGui::InputInt(prop->GetName().c_str(), (int*)&intU64Prop->Value);
    }

    void CreatePropGUI_Float(BaseProp* prop)
    {
        PropFloat* floatProp = (PropFloat*)prop;
        ImGui::InputFloat(prop->GetName().c_str(), (float*)&floatProp->Value);
    }

    void CreatePropGUI_Double(BaseProp* prop)
    {
        PropDouble* doubleProp = (PropDouble*)prop;
        ImGui::InputDouble(prop->GetName().c_str(), (double*)&doubleProp->Value);
    }

    void CreatePropGUI_String(BaseProp* prop)
    {
        PropDouble* doubleProp = (PropDouble*)prop;
        ImGui::InputText(prop->GetName().c_str(), (std::string*)&doubleProp->Value);
    }

    void CreatePropGUI_Vec2(BaseProp* prop)
    {
        PropVec2* vec2Prop = (PropVec2*)prop;
        ImGui::InputFloat2(prop->GetName().c_str(), (float*)&vec2Prop->Value);
    }

    void CreatePropGUI_Vec3(BaseProp* prop)
    {
        PropVec3* vec3Prop = (PropVec3*)prop;
        ImGui::InputFloat3(prop->GetName().c_str(), (float*)&vec3Prop->Value);
    }

    void CreatePropGUI_Bool(BaseProp* prop)
    {
        PropBool* boolProp = (PropBool*)prop;
        ImGui::Checkbox(prop->GetName().c_str(), (bool*)&boolProp->Value);
    }

    void CreatePropGUI_Color(BaseProp* prop)
    {
        PropColor* colorProp = (PropColor*)prop;
        ImGui::ColorEdit4(prop->GetName().c_str(), (float*)&colorProp->Value);
    }

    void CreatePropGUI_Texture(BaseProp* prop)
    {
        PropTexture* textureProp = (PropTexture*)prop;

        if (textureProp->Value != nullptr)
        {
            ImGui::Image((ImTextureID)(INT_PTR)textureProp->Value->GetHandle(), ImVec2(64.0f, 64.0f));
        }
        else
        {
            auto defaultTexture = Application::Get().GetResources().Textures[UID(RES_TEXTURE_DEFAULT)];
            ImGui::Image((ImTextureID)(INT_PTR)defaultTexture->GetHandle(), ImVec2(64.0f, 64.0f));
        }

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
        ImGui::Text(prop->GetName().c_str());
    }

    void CreatePropGUI_Entity(BaseProp* prop)
    {
        PropEntity* entityProp = (PropEntity*)prop;
        Entity ent = entityProp->Value;
        if (ent != ENTITY_INVALID)
        {
            ImGui::Text("%ull", ent);
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
