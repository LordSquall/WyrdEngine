#include "wyrdpch.h"

/* local includes  */
#include "core/Application.h"
#include "core/ecs/Components.h"
#include "core/behaviour/ScriptedClass.h"
#include "serial/TypeSerialisers.h"
#include "serial/ComponentSerialiserFactory.h"
#include "serial/CustomComponentSerialisationFunctions.h"

namespace Wyrd
{
    template <typename T>
    T Get(jsonxx::Object& obj, const std::string& name, T defaultValue) {
        if (obj.has<T>(name))
        {
            return obj.get<T>(name);
        }

        return defaultValue;
    }

    void ComponentSerialiserFactory::Serialise(jsonxx::Object & obj, Wyrd::MetaDataComponent* data)
    {
        obj << "name" << data->name;
        obj << "uid" << data->uid;
    }

    void ComponentSerialiserFactory::Serialise(jsonxx::Object & obj, Wyrd::Transform2DComponent* data)
    {
        obj << "position" << data->position;
        obj << "rotation" << data->rotation;
        obj << "scale" << data->scale;
    }

    void ComponentSerialiserFactory::Serialise(jsonxx::Object & obj, Wyrd::SpriteComponent* data)
    {
        obj << "enabled" << data->enabled;
        obj << "sprite" << data->sprite;
        obj << "size" << data->size;
        obj << "tint" << data->tint;
    }

    void ComponentSerialiserFactory::Serialise(jsonxx::Object & obj, Wyrd::ScriptComponent* data)
    {
        obj << "enabled" << data->enabled;
        obj << "scriptId" << data->scriptId;
        obj << "instanceId" << data->instanceId;
        CustomSerialisation_ScriptComponent(obj, data);
    }

    void ComponentSerialiserFactory::Serialise(jsonxx::Object & obj, Wyrd::RelationshipComponent* data)
    {
        obj << "first" << data->first;
        obj << "previous" << data->previous;
        obj << "next" << data->next;
        obj << "parent" << data->parent;
        obj << "childrenCnt" << data->childrenCnt;
        obj << "depth" << data->depth;
        obj << "remove" << data->remove;
    }

    void ComponentSerialiserFactory::Serialise(jsonxx::Object & obj, Wyrd::CameraComponent* data)
    {
        obj << "enabled" << data->enabled;
        obj << "scriptId" << data->scriptId;
        obj << "instanceId" << data->instanceId;
    }




    void ComponentSerialiserFactory::Deserialise(jsonxx::Object & obj, Wyrd::MetaDataComponent* data)
    {
        data->name << obj.get<jsonxx::String>("name", DecodeString("Unknown Entity"));
        data->uid << obj.get<jsonxx::String>("uid", UID());
    }

    void ComponentSerialiserFactory::Deserialise(jsonxx::Object & obj, Wyrd::Transform2DComponent* data)
    {
        data->position << obj.get<jsonxx::Object>("position", DecodeVec2("0,0"));
        data->rotation = obj.get<jsonxx::Number>("rotation", 0.0f);
        data->scale << obj.get<jsonxx::Object>("scale", DecodeVec2("1,1"));
    }

    void ComponentSerialiserFactory::Deserialise(jsonxx::Object & obj, Wyrd::SpriteComponent* data)
    {
        data->enabled = obj.get<jsonxx::Boolean>("enabled", true);
        data->sprite << obj.get<jsonxx::String>("sprite", UID());
        data->size << obj.get<jsonxx::Object>("size", DecodeVec2("64,64"));
        data->tint << obj.get<jsonxx::Object>("tint", DecodeColor("1,1,1,1"));
    }

    void ComponentSerialiserFactory::Deserialise(jsonxx::Object & obj, Wyrd::ScriptComponent* data)
    {
        data->enabled = obj.get<jsonxx::Boolean>("enabled", true);
        data->scriptId << obj.get<jsonxx::String>("scriptId", UID());
        data->instanceId = obj.get<jsonxx::Number>("instanceId", 0);
        CustomDeserialisation_ScriptComponent(data, obj);
    }

    void ComponentSerialiserFactory::Deserialise(jsonxx::Object & obj, Wyrd::RelationshipComponent* data)
    {
        data->first = obj.get<jsonxx::Number>("first", ENTITY_INVALID);
        data->previous = obj.get<jsonxx::Number>("previous", ENTITY_INVALID);
        data->next = obj.get<jsonxx::Number>("next", ENTITY_INVALID);
        data->parent = obj.get<jsonxx::Number>("parent", ENTITY_INVALID);
        data->childrenCnt = obj.get<jsonxx::Number>("childrenCnt", 0);
        data->depth = obj.get<jsonxx::Number>("depth", 0);
        data->remove = obj.get<jsonxx::Boolean>("remove", false);
    }

    void ComponentSerialiserFactory::Deserialise(jsonxx::Object & obj, Wyrd::CameraComponent* data)
    {
        data->enabled = obj.get<jsonxx::Boolean>("enabled", true);
        data->scriptId << obj.get<jsonxx::String>("scriptId", UID());
        data->instanceId = obj.get<jsonxx::Number>("instanceId", 0);
    }


}