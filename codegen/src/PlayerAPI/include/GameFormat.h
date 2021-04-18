/// Auto Generated
/// Date: Apr 2021 Sun 20:08:01

#pragma once

#include <osrpch.h>

#include <core/UID.h>

struct GameFormat_Config
{
    std::string version;
    std::string name;
};

struct GameFormat_Display
{
    uint32_t width;
    uint32_t height;
};

struct GameFormat_SceneListings
{
    Osiris::UID initialScene;
    uint32_t sceneUIDs_cnt;
    std::vector<Osiris::UID> sceneUIDs;
};

