#pragma once

/* core wyrd includes */
#include <wyrdpch.h>

/* local includes */
#include "views/PropertiesViewer/ComponentViews/ScriptProperties/ScriptPropertyViewFactory.h"

namespace Wyrd::Editor
{
    std::map<std::string, ScriptPropertyViewFactory::CreatePropertyViewFunc>* GetPropertyViews()
    {
        static std::map<std::string, ScriptPropertyViewFactory::CreatePropertyViewFunc> propertyViews;
        return &propertyViews;
    }

    bool ScriptPropertyViewFactory::Register(const std::string name,
        CreatePropertyViewFunc funcCreate)
    {
        auto views = GetPropertyViews();
        if (auto it = views->find(name); it == views->end())
        {
            (*views)[name] = funcCreate;
            return true;
        }
        return false;
    }

    bool ScriptPropertyViewFactory::Create(const std::shared_ptr<ScriptProperty>& prop, void* data)
    {
        auto views = GetPropertyViews();
        auto factoryName = prop->GetFactoryName();
        if (auto it = views->find(prop->GetFactoryName()); it != views->end())
        {
            it->second(prop, data);
            return true;
        }
        return false;
    }
}