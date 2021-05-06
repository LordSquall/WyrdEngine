/// Auto Generated
/// Date: May 2021 Wed 21:51:31

#pragma once

#include <wyrdpch.h>

#include <core/UID.h>

struct SceneFormat_Config
{
    std::string version;
    std::string name;
    Wyrd::UID camera;
};

struct SceneFormat_Transform2DComponent
{
    float x;
    float y;
    float angle;
    float scaleX;
    float scaleY;
};

struct SceneFormat_Transform3DComponent
{
    float x;
    float y;
    float z;
    float angleX;
    float angleY;
    float angleZ;
    float scaleX;
    float scaleY;
    float scaleZ;
};

struct SceneFormat_SpriteComponent
{
    float x;
    float y;
    float width;
    float height;
    Wyrd::UID texture;
};

struct SceneFormat_ScriptComponent
{
    Wyrd::UID script;
};

struct SceneFormat_CameraComponent
{
    Wyrd::UID uid;
    float size;
    float width;
    float height;
};

struct SceneFormat_Component
{
    int32_t type;
    union ComponentDef
    {
        SceneFormat_SpriteComponent sprite;      
        SceneFormat_ScriptComponent script;      
        SceneFormat_CameraComponent camera;      
        ComponentDef() { }
    } componentDef;
};

struct SceneFormat_GameObject
{
    std::string name;
    Wyrd::UID uid;
    int32_t transformType;
    uint32_t components_cnt;
    std::vector<SceneFormat_Component> components;
    union TransformDef
    {
        SceneFormat_Transform2DComponent transform2D;      
        SceneFormat_Transform3DComponent transform3D;      
        TransformDef() { }
    } transformDef;
};

struct SceneFormat_Layer
{
    std::string name;
    uint32_t gameObjects_cnt;
    std::vector<SceneFormat_GameObject> gameObjects;
};

struct SceneFormat_LayerConfig
{
    uint32_t layers_cnt;
    std::vector<SceneFormat_Layer> layers;
};

