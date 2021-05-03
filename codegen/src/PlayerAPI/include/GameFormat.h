/// Auto Generated
/// Date: May 2021 Mon 23:36:01

#pragma once

#include <wyrdpch.h>

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
    Wyrd::UID initialScene;
    uint32_t sceneUIDs_cnt;
    std::vector<Wyrd::UID> sceneUIDs;
};

