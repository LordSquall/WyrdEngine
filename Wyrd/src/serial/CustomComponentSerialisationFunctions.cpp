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

    void CustomSerialisation_MaterialComponent(jsonxx::Object& obj, Wyrd::MaterialComponent* data)
    {
        jsonxx::Object propertiesSet;
        if (data->properties != nullptr)
        {
            for (auto& [name, prop] : *data->properties)
            {
                prop->Serialise(propertiesSet);
            }
        }
        
        obj << "properties" << propertiesSet;
    }

    void CustomDeserialisation_MaterialComponent(Wyrd::MaterialComponent* data, jsonxx::Object& obj)
    {
        if (obj.has<jsonxx::Object>("properties"))
        {
            jsonxx::Object propsObject = obj.get<jsonxx::Object>("properties");

            BasePropMapRef propMap = PropFactory::CreatePropMap(propsObject);

            data->properties.swap(propMap);
        }
    }

    void CustomSerialisation_ScriptComponent(jsonxx::Object& obj, Wyrd::ScriptComponent* data)
    {

        jsonxx::Object propertiesSet;
        if (data->properties != nullptr)
        {
            for (auto& [name, prop] : *data->properties)
            {
                prop->Serialise(propertiesSet);
            }
        }

        obj << "properties" << propertiesSet;
    }

    void CustomDeserialisation_ScriptComponent(Wyrd::ScriptComponent* data, jsonxx::Object& obj)
    {
        if (obj.has<jsonxx::Object>("properties"))
        {
            jsonxx::Object propsObject = obj.get<jsonxx::Object>("properties");

            BasePropMapRef propMap = PropFactory::CreatePropMap(propsObject);

            data->properties.swap(propMap);
        }
    }
}