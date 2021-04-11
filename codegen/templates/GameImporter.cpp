/// Auto Generated
/// Date: Apr 2021 Sun 10:20:05

#pragma once

#include <osrpch.h>

void Read(std::ifstream& s, GameFormat_Config& d);
{
    Read(s, d.version);
};

void Read(std::ifstream& s, GameFormat_Display& d);
{
    Read(s, d.width);
    Read(s, d.height);
};

void Read(std::ifstream& s, GameFormat_SceneListings& d);
{
    Read(s, d.initialScene);
    Read(s, d.sceneUIDs_cnt);
    for (uint32_t i = 0; i < d.sceneUIDs_cnt; i++)
    {
        GameFormat_UID item;
        Read(s, sceneUIDs);
        d.sceneUIDs.push_back(item);
    }
};

