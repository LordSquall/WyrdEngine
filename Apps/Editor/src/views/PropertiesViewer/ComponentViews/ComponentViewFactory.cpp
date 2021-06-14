#pragma once

/* Local includes */
#include "wyrdpch.h"
#include "ComponentViewFactory.h"

namespace Wyrd::Editor
{
    std::map<std::string, ComponentViewFactory::ViewRenderFunc>* GetFunctions()
    {
        static std::map<std::string, ComponentViewFactory::ViewRenderFunc> functions;
        return &functions;
    }

    bool ComponentViewFactory::Register(const std::string name,
        ViewRenderFunc renderFunc)
    {
        auto functions = GetFunctions();
        if (auto it = functions->find(name); it == functions->end())
        {
            (*functions)[name] = renderFunc;
            return true;
        }
        return false;
    }

    void ComponentViewFactory::Create(const std::string& name, Entity e, void* data)
    {
        auto functions = GetFunctions();
        if (auto it = functions->find(name); it != functions->end())
            return it->second(e, data);
    }
}