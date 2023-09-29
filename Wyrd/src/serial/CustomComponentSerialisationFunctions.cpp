#include "wyrdpch.h"

/* local includes  */
#include "core/Application.h"
#include "core/ecs/Components.h"
#include "core/behaviour/ScriptedClass.h"
#include "core/Log.h"
#include "core/support/MaterialHelperFuncs.h"
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
        // retrieve the material we are using 
        MaterialHelperFuncs::AssignToComponent(data);

        if (obj.has<jsonxx::Object>("properties"))
        {
                
           jsonxx::Object propsObject = obj.get<jsonxx::Object>("properties");

           BasePropMapRef propMap = PropFactory::CreatePropMap(propsObject);

           PropFactory::MergePropMaps(data->properties, propMap);
        }
    }


    void CustomSerialisation_MaterialComponent(std::ofstream& stream, Wyrd::MaterialComponent* data)
    {
        // Write properties stream;
        size_t propertyCount = 0;
        if (data->properties != nullptr)
        {
            propertyCount = data->properties->size();
            stream.write((char*)&propertyCount, sizeof(size_t));
            for (auto& [name, prop] : *data->properties)
            {
                prop->Serialise(stream);
            }
        }
        else
        {
            stream.write((char*)&propertyCount, sizeof(size_t));
        }
    }

    void CustomDeserialisation_MaterialComponent(std::ifstream& stream, Wyrd::MaterialComponent* data)
    {
        // Read properties steam;
        BasePropMapRef propMap = PropFactory::CreatePropMap(stream);

        data->properties.swap(propMap);
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


    void CustomSerialisation_ScriptComponent(std::ofstream& stream, Wyrd::ScriptComponent* data)
    {
        // Write properties stream;
        size_t propertyCount = 0;
        if (data->properties != nullptr)
        {
            propertyCount = data->properties->size();
            stream.write((char*)&propertyCount, sizeof(size_t));
            for (auto& [name, prop] : *data->properties)
            {
                prop->Serialise(stream);
            }
        }
        else
        {
            stream.write((char*)&propertyCount, sizeof(size_t));
        }
    }

    void CustomDeserialisation_ScriptComponent(std::ifstream& stream, Wyrd::ScriptComponent* data)
    {
        // Read properties steam;
        BasePropMapRef propMap = PropFactory::CreatePropMap(stream);

        data->properties.swap(propMap);
    }
}