/// Auto Generated
/// Date: Apr 2021 Sun 20:08:01

#pragma once

#include <osrpch.h>

#include "CommonFunctions.h"

#include "SceneFormat.h"

void Read(std::ifstream& s, SceneFormat_Config& d)
{
    Read(s, d.version);
    Read(s, d.name);
    Read(s, d.camera);
};

void Write(std::ofstream& s, SceneFormat_Config& d)
{
    Write(s, d.version);
    Write(s, d.name);
    Write(s, d.camera);
};

void Read(std::ifstream& s, SceneFormat_Transform2DComponent& d)
{
    Read(s, d.x);
    Read(s, d.y);
    Read(s, d.angle);
    Read(s, d.scaleX);
    Read(s, d.scaleY);
};

void Write(std::ofstream& s, SceneFormat_Transform2DComponent& d)
{
    Write(s, d.x);
    Write(s, d.y);
    Write(s, d.angle);
    Write(s, d.scaleX);
    Write(s, d.scaleY);
};

void Read(std::ifstream& s, SceneFormat_Transform3DComponent& d)
{
    Read(s, d.x);
    Read(s, d.y);
    Read(s, d.z);
    Read(s, d.angleX);
    Read(s, d.angleY);
    Read(s, d.angleZ);
    Read(s, d.scaleX);
    Read(s, d.scaleY);
    Read(s, d.scaleZ);
};

void Write(std::ofstream& s, SceneFormat_Transform3DComponent& d)
{
    Write(s, d.x);
    Write(s, d.y);
    Write(s, d.z);
    Write(s, d.angleX);
    Write(s, d.angleY);
    Write(s, d.angleZ);
    Write(s, d.scaleX);
    Write(s, d.scaleY);
    Write(s, d.scaleZ);
};

void Read(std::ifstream& s, SceneFormat_SpriteComponent& d)
{
    Read(s, d.x);
    Read(s, d.y);
    Read(s, d.width);
    Read(s, d.height);
};

void Write(std::ofstream& s, SceneFormat_SpriteComponent& d)
{
    Write(s, d.x);
    Write(s, d.y);
    Write(s, d.width);
    Write(s, d.height);
};

void Read(std::ifstream& s, SceneFormat_CameraComponent& d)
{
    Read(s, d.uid);
    Read(s, d.size);
    Read(s, d.width);
    Read(s, d.height);
};

void Write(std::ofstream& s, SceneFormat_CameraComponent& d)
{
    Write(s, d.uid);
    Write(s, d.size);
    Write(s, d.width);
    Write(s, d.height);
};

void Read(std::ifstream& s, SceneFormat_Component& d)
{
    Read(s, d.type);
    switch (d.type)
    {
        case 3:
            Read(s, d.componentDef.sprite);
            break;
        case 6:
            Read(s, d.componentDef.camera);
            break;
    }
};

void Write(std::ofstream& s, SceneFormat_Component& d)
{
    Write(s, d.type);
    switch (d.type)
    {
        case 3:
            Write(s, d.componentDef.sprite);
            break;
        case 6:
            Write(s, d.componentDef.camera);
            break;
    }
};

void Read(std::ifstream& s, SceneFormat_GameObject& d)
{
    Read(s, d.name);
    Read(s, d.uid);
    Read(s, d.transformType);
    Read(s, d.components_cnt);
    for (uint32_t i = 0; i < d.components_cnt; i++)
    {
        SceneFormat_Component item;
        Read(s, item);
        d.components.push_back(item);
    }
    switch (d.transformType)
    {
        case 1:
            Read(s, d.transformDef.transform2D);
            break;
        case 2:
            Read(s, d.transformDef.transform3D);
            break;
    }
};

void Write(std::ofstream& s, SceneFormat_GameObject& d)
{
    Write(s, d.name);
    Write(s, d.uid);
    Write(s, d.transformType);
    Write(s, d.components_cnt);
    for (uint32_t i = 0; i < d.components_cnt; i++)
    {
        Write(s, d.components[i]);
    }
    switch (d.transformType)
    {
        case 1:
            Write(s, d.transformDef.transform2D);
            break;
        case 2:
            Write(s, d.transformDef.transform3D);
            break;
    }
};

void Read(std::ifstream& s, SceneFormat_Layer& d)
{
    Read(s, d.name);
    Read(s, d.gameObjects_cnt);
    for (uint32_t i = 0; i < d.gameObjects_cnt; i++)
    {
        SceneFormat_GameObject item;
        Read(s, item);
        d.gameObjects.push_back(item);
    }
};

void Write(std::ofstream& s, SceneFormat_Layer& d)
{
    Write(s, d.name);
    Write(s, d.gameObjects_cnt);
    for (uint32_t i = 0; i < d.gameObjects_cnt; i++)
    {
        Write(s, d.gameObjects[i]);
    }
};

void Read(std::ifstream& s, SceneFormat_LayerConfig& d)
{
    Read(s, d.layers_cnt);
    for (uint32_t i = 0; i < d.layers_cnt; i++)
    {
        SceneFormat_Layer item;
        Read(s, item);
        d.layers.push_back(item);
    }
};

void Write(std::ofstream& s, SceneFormat_LayerConfig& d)
{
    Write(s, d.layers_cnt);
    for (uint32_t i = 0; i < d.layers_cnt; i++)
    {
        Write(s, d.layers[i]);
    }
};

