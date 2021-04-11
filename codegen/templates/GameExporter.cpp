/// Auto Generated
/// Date: Apr 2021 Sun 10:20:05

#pragma once

#include <osrpch.h>

void Write(std::ofstream& s, GameFormat_Config& d);
{
    Write(s, d.version);
};

void Write(std::ofstream& s, GameFormat_Display& d);
{
    Write(s, d.width);
    Write(s, d.height);
};

void Write(std::ofstream& s, GameFormat_SceneListings& d);
{
    Write(s, d.initialScene);
    Write(s, d.sceneUIDs_cnt);
    for (uint32_t i = 0; i < d.sceneUIDs_cnt; i++)
    {
        Write(s, d.sceneUIDs[i]);
    }
};

