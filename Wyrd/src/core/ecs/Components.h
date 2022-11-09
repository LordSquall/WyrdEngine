#pragma once

#include "wyrdpch.h"
#include "core/export.h"
#include "core/UID.h"
#include "core/Structures.h"
#include "core/TypeDefs.h"
#include "core/ResourcesUIDs.h"
#include "core/maths/Rect.h"
#include "core/ecs/ECS.h"
#include "core/behaviour/Properties/ScriptProperty.h"

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

        MetaDataComponent() : name(), uid() {} 
    };


    struct WYRD_LIBRARY_API Transform2DComponent
    {
        Wyrd::Vector2 position;
        float rotation;
        Wyrd::Vector2 scale;

        Transform2DComponent() : position(0.0,0.0), rotation(0.0f), scale(0.0,0.0) {} 
    };


    struct WYRD_LIBRARY_API SpriteComponent
    {
        bool enabled;
        Wyrd::UID sprite;
        Wyrd::Vector2 size;
        Wyrd::Color tint;

        SpriteComponent() : enabled(false), sprite(), size(0.0,0.0), tint(1,1,1,1) {} 
    };


    struct WYRD_LIBRARY_API ScriptComponent
    {
        bool enabled;
        Wyrd::UID scriptId;
        int32_t instanceId;
        ScriptPropertiesMapRef properties;

        ScriptComponent() : enabled(false), scriptId(), instanceId(0), properties(nullptr) {} 
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
    };


    struct WYRD_LIBRARY_API CameraComponent
    {
        bool enabled;
        Wyrd::UID scriptId;
        int32_t instanceId;

        CameraComponent() : enabled(false), scriptId(), instanceId(0) {} 
    };

};