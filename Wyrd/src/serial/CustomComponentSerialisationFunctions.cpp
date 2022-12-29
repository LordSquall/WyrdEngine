#include "wyrdpch.h"

/* local includes  */
#include "core/Application.h"
#include "core/ecs/Components.h"
#include "core/behaviour/ScriptedClass.h"
#include "core/Log.h"
#include "serial/TypeSerialisers.h"
#include "serial/ComponentSerialiserFactory.h"

namespace Wyrd
{
    void CustomSerialisation_ScriptComponent(jsonxx::Object& obj, Wyrd::ScriptComponent* data)
    {
        jsonxx::Object propertiesSet;

        if (data->properties != nullptr)
        {
            for each (auto & prop in *data->properties)
            {
                prop.second->Serialise(propertiesSet);
            }

            obj << "properties" << propertiesSet;
        }
    }

    void CustomDeserialisation_ScriptComponent(Wyrd::ScriptComponent* data, jsonxx::Object& obj)
    {
        if (obj.has<jsonxx::Object>("properties"))
        {
            // retrieve a copy of the default properties from the assigned script
            std::shared_ptr<ScriptedClass> scriptClass = Application::Get().GetBehaviour().GetCustomClassByUID(data->scriptId);

            if (scriptClass != nullptr)
            {
                data->properties = scriptClass->GetPropertiesCopy();

                jsonxx::Object propertiesSet = obj.get<jsonxx::Object>("properties");

                for (auto& prop : propertiesSet.kv_map())
                {
                    // retrieve the script property
                    std::shared_ptr<ScriptProperty> scriptProp = (*data->properties)[prop.first];
                    if (scriptProp != nullptr)
                    {
                        auto type = scriptProp->GetTypeName();
                        if (type == "Single")
                        {
                            const jsonxx::Number& v = prop.second->get<jsonxx::Number>();
                            float d = (float)v;
                            scriptProp->Set(&d);
                        }
                        else if (type == "Int32")
                        {
                            const jsonxx::Number& v = prop.second->get<jsonxx::Number>();
                            INT32 d = (INT32)v;
                            scriptProp->Set(&d);
                        }
                        else if (type == "String")
                        {
                            const jsonxx::String& v = prop.second->get<jsonxx::String>();
                            std::string d = (std::string)v;
                            scriptProp->Set(&d);
                        }
                        else if (type == "Boolean")
                        {
                            const jsonxx::Boolean& v = prop.second->get<jsonxx::Boolean>();
                            bool d = (bool)v;
                            scriptProp->Set(&d);
                        }
                        else if (type == "Color")
                        {
                            if (!prop.second->is<jsonxx::Null>())
                            {
                                const jsonxx::Object& v = prop.second->get<jsonxx::Object>();
                                Color d;
                                d << v;
                                scriptProp->Set(&d);
                            }
                            else
                            {
                                Color d(1.0f, 1.0f, 1.0f);
                                scriptProp->Set(&d);
                            }
                        }
                        else if (type == "Vector2")
                        {
                            const jsonxx::Object& v = prop.second->get<jsonxx::Object>();
                            Vector2 d(v.get<jsonxx::Number>("x"), v.get<jsonxx::Number>("y"));
                            scriptProp->Set(&d);
                        }
                        else if (type == "Vector3")
                        {
                            const jsonxx::Object& v = prop.second->get<jsonxx::Object>();
                            Vector3 d(v.get<jsonxx::Number>("x"), v.get<jsonxx::Number>("y"), v.get<jsonxx::Number>("z"));
                            scriptProp->Set(&d);
                        }
                    }
                    else
                    {
                        WYRD_CORE_WARN("Unable to serialise script prop {0}. Indicates the script has been modified outside of the Editor!", prop.first);
                    }
                }
            }
        }
    }
}