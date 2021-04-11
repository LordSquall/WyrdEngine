/// Auto Generated
/// Date: Apr 2021 Sun 10:20:05

#pragma once

#include <osrpch.h>

struct GameFormat_Config
{
    std::string version;
};

struct GameFormat_Display
{
    uint32_t width;
    uint32_t height;
};

struct GameFormat_SceneListings
{
    std::string initialScene;
    uint32_t sceneUIDs_cnt;
    std::vector<Osiris::UID> sceneUIDs;
};

