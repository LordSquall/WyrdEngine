#pragma once

#include "wyrdpch.h"
#include "core/export.h"
#include "core/UID.h"
#include "core/Structures.h"
#include "core/TypeDefs.h"
#include "core/ResourcesUIDs.h"
#include "core/maths/Rect.h"
#include "core/ecs/ECS.h"
#include "core/ecs/ComponentPool.h"

#include <glm/glm.hpp>

#define METADATACOMPONENT_NAME_LENGTH 255

namespace Wyrd
{
	
    /**
     * Default Component to all entity to provide basic ID information
    **/ 
    struct WYRD_LIBRARY_API MetaDataComponent
    {
        char name[255];
        Wyrd::UID uid;

        MetaDataComponent() : name(), uid(UID()) {} 

        static void ResetFunc(ComponentPool* pool, Entity entity) {};
    };


    struct WYRD_LIBRARY_API Transform2DComponent
    {
        Wyrd::Vector2 position;
        float rotation;
        Wyrd::Vector2 scale;

        Transform2DComponent() : position(0,0), rotation(0.0f), scale(1,1) {} 

        static void ResetFunc(ComponentPool* pool, Entity entity) {}
    };



    struct WYRD_LIBRARY_API Transform3DComponent
    {
        Wyrd::Vector3 position;
        Wyrd::Vector3 rotation;
        Wyrd::Vector3 scale;
        glm::mat4 modelMatrix;

        Transform3DComponent() : position(0,0,0), rotation(0,0,0), scale(1,1,1), modelMatrix(glm::mat4(1.0)) {}

        static void ResetFunc(ComponentPool* pool, Entity entity) {}
    };


    struct WYRD_LIBRARY_API MeshRendererComponent
    {
        bool enabled;
        Wyrd::UID model;

        MeshRendererComponent() : enabled(true), model(RES_MODEL_3D_DEFAULT) {}

        static void ResetFunc(ComponentPool* pool, Entity entity) {}
    };


    struct WYRD_LIBRARY_API MaterialComponent
    {
        bool enabled;
        Wyrd::Color color;
        Wyrd::UID material;
        BasePropMapRef properties;

        MaterialComponent() : enabled(true), color(1,1,1,1), material(RES_MATERIAL_3D_DEFAULT), properties(nullptr) {}

        static void ResetFunc(ComponentPool* pool, Entity entity) 
        {
            MaterialComponent* c = (MaterialComponent*)pool->get(entity);
            c->properties = nullptr;
        }
    };


    struct WYRD_LIBRARY_API SpriteComponent
    {
        bool enabled;
        Wyrd::UID sprite;
        Wyrd::Vector2 position;
        Wyrd::Vector2 size;
        Wyrd::Vector2 tiling;
        Wyrd::Color color;

        SpriteComponent() : enabled(true), sprite(UID()), position(0,0), size(64,64), tiling(1,1), color(1,1,1,1) {}

        static void ResetFunc(ComponentPool* pool, Entity entity) {}
    };


    struct WYRD_LIBRARY_API ScriptComponent
    {
        bool enabled;
        Wyrd::UID scriptId;
        int32_t instanceId;
        BasePropMapRef properties;

        ScriptComponent() : enabled(true), scriptId(UID()), instanceId(0), properties(nullptr) {}

        static void ResetFunc(ComponentPool* pool, Entity entity) {}
    };


    struct WYRD_LIBRARY_API RelationshipComponent
    {
        Wyrd::Entity first;
        Wyrd::Entity previous;
        Wyrd::Entity next;
        Wyrd::Entity parent;
        uint32_t childrenCnt;
        int32_t depth;
        bool remove;

        RelationshipComponent() : first(ENTITY_INVALID), previous(ENTITY_INVALID), next(ENTITY_INVALID), parent(ENTITY_INVALID), childrenCnt(0), depth(0), remove(false) {}

        static void ResetFunc(ComponentPool* pool, Entity entity) {}
    };


    struct WYRD_LIBRARY_API CameraComponent
    {
        Wyrd::Rect viewport;
        float aspectRatio;
        Wyrd::Vector2 size;
        float nearPlane;
        float farPlane;
        float aspect;
        float fov;

        CameraComponent() : viewport(0,0,0,0), aspectRatio(1.0f), size(0,0), nearPlane(0.1f), farPlane(100.0f), aspect(1.0f), fov(45.0f) {}

        static void ResetFunc(ComponentPool* pool, Entity entity) {}
    };

};