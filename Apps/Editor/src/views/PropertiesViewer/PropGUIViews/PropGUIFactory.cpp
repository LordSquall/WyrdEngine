/* Local includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "views/PropertiesViewer/PropGUIViews/PropGUIFactory.h"
#include "properties/BaseProp.h"
#include "core/Application.h"

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
            ImGui::Image((ImTextureID)(INT_PTR)textureProp->Value->GetHandle(), ImVec2((float)textureProp->Value->GetWidth(), (float)textureProp->Value->GetHeight()));
        }
        else
        {
            auto defaultTexture = Application::Get().GetResources().Textures[UID(RES_TEXTURE_DEFAULT)];
            ImGui::Image((ImTextureID)(INT_PTR)defaultTexture->GetHandle(), ImVec2((float)defaultTexture->GetWidth(), (float)defaultTexture->GetHeight()));
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
    }

    std::map<std::string, PropGUIFactory::DrawPropGUIFunc>* PropGUIFactory::GetPropsGUI()
    {
        static std::map<std::string, PropGUIFactory::DrawPropGUIFunc> properties{
            { "Int", CreatePropGUI_Int },
            { "Int32", CreatePropGUI_Int32 },
            { "UInt32", CreatePropGUI_UInt32 },
            { "Int64", CreatePropGUI_Int64 },
            { "UInt64", CreatePropGUI_UInt64 },
            { "Vec2", CreatePropGUI_Vec2 },
            { "Vec3", CreatePropGUI_Vec3 },
            { "Color", CreatePropGUI_Color },
            { "Texture", CreatePropGUI_Texture }
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

    void PropGUIFactory::DrawMissingProp(const std::string& name)
    {
        ImGui::Text("Missing GUI for prop %s", name.c_str());
    }
}
