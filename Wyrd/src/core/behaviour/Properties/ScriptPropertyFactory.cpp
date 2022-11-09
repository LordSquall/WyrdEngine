/* Local includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

namespace Wyrd
{
    std::map<std::string, ScriptPropertyFactory::CreatePropertyFunc>* GetProperties()
    {
        static std::map<std::string, ScriptPropertyFactory::CreatePropertyFunc> properties;
        return &properties;
    }

    bool ScriptPropertyFactory::Register(const std::string name,
        CreatePropertyFunc funcCreate)
    {
        auto properties = GetProperties();
        if (auto it = properties->find(name); it == properties->end())
        {
            (*properties)[name] = funcCreate;
            return true;
        }
        return false;
    }

    std::unique_ptr<ScriptProperty>
        ScriptPropertyFactory::Create(const std::string& name)
    {
        auto properties = GetProperties();
        if (auto it = properties->find(name); it != properties->end())
            return it->second(); // call the createFunc

        WYRD_CORE_ERROR("Unable to create script property of type {0}", name);

        return nullptr;
    }
}