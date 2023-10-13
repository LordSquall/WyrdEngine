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

    void ComponentSerialiserFactory::Serialise(std::ofstream& stream, Wyrd::MetaDataComponent* data)
    {
        stream.write((char*)&data->name, sizeof(char) * 255);
        stream.write((char*)&data->uid, sizeof(Wyrd::UID));
    }

    void ComponentSerialiserFactory::Serialise(jsonxx::Object & obj, Wyrd::Transform2DComponent* data)
    {
        obj << "position" << data->position;
        obj << "rotation" << data->rotation;
        obj << "scale" << data->scale;
    }

    void ComponentSerialiserFactory::Serialise(std::ofstream& stream, Wyrd::Transform2DComponent* data)
    {
        stream.write((char*)&data->position, sizeof(Wyrd::Vector2));
        stream.write((char*)&data->rotation, sizeof(float));
        stream.write((char*)&data->scale, sizeof(Wyrd::Vector2));
    }

    void ComponentSerialiserFactory::Serialise(jsonxx::Object & obj, Wyrd::Transform3DComponent* data)
    {
        obj << "position" << data->position;
        obj << "rotation" << data->rotation;
        obj << "scale" << data->scale;
    }

    void ComponentSerialiserFactory::Serialise(std::ofstream& stream, Wyrd::Transform3DComponent* data)
    {
        stream.write((char*)&data->position, sizeof(Wyrd::Vector3));
        stream.write((char*)&data->rotation, sizeof(Wyrd::Vector3));
        stream.write((char*)&data->scale, sizeof(Wyrd::Vector3));
        stream.write((char*)&data->modelMatrix, sizeof(glm::mat4));
        stream.write((char*)&data->parentModelMatrix, sizeof(glm::mat4));
    }

    void ComponentSerialiserFactory::Serialise(jsonxx::Object & obj, Wyrd::MeshRendererComponent* data)
    {
        obj << "enabled" << data->enabled;
        obj << "model" << data->model;
    }

    void ComponentSerialiserFactory::Serialise(std::ofstream& stream, Wyrd::MeshRendererComponent* data)
    {
        stream.write((char*)&data->enabled, sizeof(bool));
        stream.write((char*)&data->model, sizeof(Wyrd::UID));
    }

    void ComponentSerialiserFactory::Serialise(jsonxx::Object & obj, Wyrd::MaterialComponent* data)
    {
        obj << "enabled" << data->enabled;
        obj << "color" << data->color;
        obj << "material" << data->material;
        CustomSerialisation_MaterialComponent(obj, data);
    }

    void ComponentSerialiserFactory::Serialise(std::ofstream& stream, Wyrd::MaterialComponent* data)
    {
        stream.write((char*)&data->enabled, sizeof(bool));
        stream.write((char*)&data->color, sizeof(Wyrd::Color));
        stream.write((char*)&data->material, sizeof(Wyrd::UID));
        CustomSerialisation_MaterialComponent(stream, data);
    }

    void ComponentSerialiserFactory::Serialise(jsonxx::Object & obj, Wyrd::SpriteComponent* data)
    {
        obj << "enabled" << data->enabled;
        obj << "sprite" << data->sprite;
        obj << "position" << data->position;
        obj << "size" << data->size;
        obj << "tiling" << data->tiling;
        obj << "color" << data->color;
    }

    void ComponentSerialiserFactory::Serialise(std::ofstream& stream, Wyrd::SpriteComponent* data)
    {
        stream.write((char*)&data->enabled, sizeof(bool));
        stream.write((char*)&data->sprite, sizeof(Wyrd::UID));
        stream.write((char*)&data->position, sizeof(Wyrd::Vector2));
        stream.write((char*)&data->size, sizeof(Wyrd::Vector2));
        stream.write((char*)&data->tiling, sizeof(Wyrd::Vector2));
        stream.write((char*)&data->color, sizeof(Wyrd::Color));
    }

    void ComponentSerialiserFactory::Serialise(jsonxx::Object & obj, Wyrd::ScriptComponent* data)
    {
        obj << "enabled" << data->enabled;
        obj << "scriptId" << data->scriptId;
        obj << "instanceId" << data->instanceId;
        CustomSerialisation_ScriptComponent(obj, data);
    }

    void ComponentSerialiserFactory::Serialise(std::ofstream& stream, Wyrd::ScriptComponent* data)
    {
        stream.write((char*)&data->enabled, sizeof(bool));
        stream.write((char*)&data->scriptId, sizeof(Wyrd::UID));
        stream.write((char*)&data->instanceId, sizeof(int32_t));
        CustomSerialisation_ScriptComponent(stream, data);
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

    void ComponentSerialiserFactory::Serialise(std::ofstream& stream, Wyrd::RelationshipComponent* data)
    {
        stream.write((char*)&data->first, sizeof(Wyrd::Entity));
        stream.write((char*)&data->previous, sizeof(Wyrd::Entity));
        stream.write((char*)&data->next, sizeof(Wyrd::Entity));
        stream.write((char*)&data->parent, sizeof(Wyrd::Entity));
        stream.write((char*)&data->childrenCnt, sizeof(uint32_t));
        stream.write((char*)&data->depth, sizeof(int32_t));
        stream.write((char*)&data->remove, sizeof(bool));
    }

    void ComponentSerialiserFactory::Serialise(jsonxx::Object & obj, Wyrd::CameraComponent* data)
    {
        obj << "viewport" << data->viewport;
        obj << "projection" << data->projection;
        obj << "aspectRatio" << data->aspectRatio;
        obj << "size" << data->size;
        obj << "nearPlane" << data->nearPlane;
        obj << "farPlane" << data->farPlane;
        obj << "aspect" << data->aspect;
        obj << "fov" << data->fov;
        obj << "top" << data->top;
        obj << "bottom" << data->bottom;
        obj << "left" << data->left;
        obj << "right" << data->right;
    }

    void ComponentSerialiserFactory::Serialise(std::ofstream& stream, Wyrd::CameraComponent* data)
    {
        stream.write((char*)&data->viewport, sizeof(Wyrd::Rect));
        stream.write((char*)&data->projection, sizeof(int32_t));
        stream.write((char*)&data->aspectRatio, sizeof(float));
        stream.write((char*)&data->size, sizeof(Wyrd::Vector2));
        stream.write((char*)&data->nearPlane, sizeof(float));
        stream.write((char*)&data->farPlane, sizeof(float));
        stream.write((char*)&data->aspect, sizeof(float));
        stream.write((char*)&data->fov, sizeof(float));
        stream.write((char*)&data->top, sizeof(float));
        stream.write((char*)&data->bottom, sizeof(float));
        stream.write((char*)&data->left, sizeof(float));
        stream.write((char*)&data->right, sizeof(float));
    }




    void ComponentSerialiserFactory::Deserialise(jsonxx::Object & obj, Wyrd::MetaDataComponent* data)
    {
        data->name << obj.get<jsonxx::String>("name", DecodeString(""));
        data->uid << obj.get<jsonxx::String>("uid", UID());
    }

    void ComponentSerialiserFactory::Deserialise(std::ifstream& stream, Wyrd::MetaDataComponent* data)
    {
        stream.read((char*)&data->name, sizeof(char) * 255);
        stream.read((char*)&data->uid, sizeof(Wyrd::UID));
    }

    void ComponentSerialiserFactory::Deserialise(jsonxx::Object & obj, Wyrd::Transform2DComponent* data)
    {
        data->position << obj.get<jsonxx::Object>("position", DecodeVec2("0,0"));
        data->rotation = obj.get<jsonxx::Number>("rotation", 0.0f);
        data->scale << obj.get<jsonxx::Object>("scale", DecodeVec2("1,1"));
    }

    void ComponentSerialiserFactory::Deserialise(std::ifstream& stream, Wyrd::Transform2DComponent* data)
    {
        stream.read((char*)&data->position, sizeof(Wyrd::Vector2));
        stream.read((char*)&data->rotation, sizeof(float));
        stream.read((char*)&data->scale, sizeof(Wyrd::Vector2));
    }

    void ComponentSerialiserFactory::Deserialise(jsonxx::Object & obj, Wyrd::Transform3DComponent* data)
    {
        data->position << obj.get<jsonxx::Object>("position", DecodeVec3("0,0,0"));
        data->rotation << obj.get<jsonxx::Object>("rotation", DecodeVec3("0,0,0"));
        data->scale << obj.get<jsonxx::Object>("scale", DecodeVec3("1,1,1"));
    }

    void ComponentSerialiserFactory::Deserialise(std::ifstream& stream, Wyrd::Transform3DComponent* data)
    {
        stream.read((char*)&data->position, sizeof(Wyrd::Vector3));
        stream.read((char*)&data->rotation, sizeof(Wyrd::Vector3));
        stream.read((char*)&data->scale, sizeof(Wyrd::Vector3));
        stream.read((char*)&data->modelMatrix, sizeof(glm::mat4));
        stream.read((char*)&data->parentModelMatrix, sizeof(glm::mat4));
    }

    void ComponentSerialiserFactory::Deserialise(jsonxx::Object & obj, Wyrd::MeshRendererComponent* data)
    {
        data->enabled = obj.get<jsonxx::Boolean>("enabled", true);
        data->model << obj.get<jsonxx::String>("model", RES_MODEL_3D_DEFAULT);
    }

    void ComponentSerialiserFactory::Deserialise(std::ifstream& stream, Wyrd::MeshRendererComponent* data)
    {
        stream.read((char*)&data->enabled, sizeof(bool));
        stream.read((char*)&data->model, sizeof(Wyrd::UID));
    }

    void ComponentSerialiserFactory::Deserialise(jsonxx::Object & obj, Wyrd::MaterialComponent* data)
    {
        data->enabled = obj.get<jsonxx::Boolean>("enabled", true);
        data->color << obj.get<jsonxx::Object>("color", DecodeColor("1,1,1,1"));
        data->material << obj.get<jsonxx::String>("material", RES_MATERIAL_3D_DEFAULT);
        CustomDeserialisation_MaterialComponent(data, obj);
    }

    void ComponentSerialiserFactory::Deserialise(std::ifstream& stream, Wyrd::MaterialComponent* data)
    {
        stream.read((char*)&data->enabled, sizeof(bool));
        stream.read((char*)&data->color, sizeof(Wyrd::Color));
        stream.read((char*)&data->material, sizeof(Wyrd::UID));
        CustomDeserialisation_MaterialComponent(stream, data);
    }

    void ComponentSerialiserFactory::Deserialise(jsonxx::Object & obj, Wyrd::SpriteComponent* data)
    {
        data->enabled = obj.get<jsonxx::Boolean>("enabled", true);
        data->sprite << obj.get<jsonxx::String>("sprite", UID());
        data->position << obj.get<jsonxx::Object>("position", DecodeVec2("0,0"));
        data->size << obj.get<jsonxx::Object>("size", DecodeVec2("64,64"));
        data->tiling << obj.get<jsonxx::Object>("tiling", DecodeVec2("1,1"));
        data->color << obj.get<jsonxx::Object>("color", DecodeColor("1,1,1,1"));
    }

    void ComponentSerialiserFactory::Deserialise(std::ifstream& stream, Wyrd::SpriteComponent* data)
    {
        stream.read((char*)&data->enabled, sizeof(bool));
        stream.read((char*)&data->sprite, sizeof(Wyrd::UID));
        stream.read((char*)&data->position, sizeof(Wyrd::Vector2));
        stream.read((char*)&data->size, sizeof(Wyrd::Vector2));
        stream.read((char*)&data->tiling, sizeof(Wyrd::Vector2));
        stream.read((char*)&data->color, sizeof(Wyrd::Color));
    }

    void ComponentSerialiserFactory::Deserialise(jsonxx::Object & obj, Wyrd::ScriptComponent* data)
    {
        data->enabled = obj.get<jsonxx::Boolean>("enabled", true);
        data->scriptId << obj.get<jsonxx::String>("scriptId", UID());
        data->instanceId = obj.get<jsonxx::Number>("instanceId", 0);
        CustomDeserialisation_ScriptComponent(data, obj);
    }

    void ComponentSerialiserFactory::Deserialise(std::ifstream& stream, Wyrd::ScriptComponent* data)
    {
        stream.read((char*)&data->enabled, sizeof(bool));
        stream.read((char*)&data->scriptId, sizeof(Wyrd::UID));
        stream.read((char*)&data->instanceId, sizeof(int32_t));
        CustomDeserialisation_ScriptComponent(stream, data);
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

    void ComponentSerialiserFactory::Deserialise(std::ifstream& stream, Wyrd::RelationshipComponent* data)
    {
        stream.read((char*)&data->first, sizeof(Wyrd::Entity));
        stream.read((char*)&data->previous, sizeof(Wyrd::Entity));
        stream.read((char*)&data->next, sizeof(Wyrd::Entity));
        stream.read((char*)&data->parent, sizeof(Wyrd::Entity));
        stream.read((char*)&data->childrenCnt, sizeof(uint32_t));
        stream.read((char*)&data->depth, sizeof(int32_t));
        stream.read((char*)&data->remove, sizeof(bool));
    }

    void ComponentSerialiserFactory::Deserialise(jsonxx::Object & obj, Wyrd::CameraComponent* data)
    {
        data->viewport << obj.get<jsonxx::Object>("viewport", DecodeRect("0,0,0,0"));
        data->projection = obj.get<jsonxx::Number>("projection", 0);
        data->aspectRatio = obj.get<jsonxx::Number>("aspectRatio", 1.0f);
        data->size << obj.get<jsonxx::Object>("size", DecodeVec2("0,0"));
        data->nearPlane = obj.get<jsonxx::Number>("nearPlane", 0.1f);
        data->farPlane = obj.get<jsonxx::Number>("farPlane", 100.0f);
        data->aspect = obj.get<jsonxx::Number>("aspect", 1.0f);
        data->fov = obj.get<jsonxx::Number>("fov", 45.0f);
        data->top = obj.get<jsonxx::Number>("top", 1.0f);
        data->bottom = obj.get<jsonxx::Number>("bottom", 1.0f);
        data->left = obj.get<jsonxx::Number>("left", 1.0f);
        data->right = obj.get<jsonxx::Number>("right", 1.0f);
    }

    void ComponentSerialiserFactory::Deserialise(std::ifstream& stream, Wyrd::CameraComponent* data)
    {
        stream.read((char*)&data->viewport, sizeof(Wyrd::Rect));
        stream.read((char*)&data->projection, sizeof(int32_t));
        stream.read((char*)&data->aspectRatio, sizeof(float));
        stream.read((char*)&data->size, sizeof(Wyrd::Vector2));
        stream.read((char*)&data->nearPlane, sizeof(float));
        stream.read((char*)&data->farPlane, sizeof(float));
        stream.read((char*)&data->aspect, sizeof(float));
        stream.read((char*)&data->fov, sizeof(float));
        stream.read((char*)&data->top, sizeof(float));
        stream.read((char*)&data->bottom, sizeof(float));
        stream.read((char*)&data->left, sizeof(float));
        stream.read((char*)&data->right, sizeof(float));
    }


}