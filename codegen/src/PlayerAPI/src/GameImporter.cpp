/// Auto Generated
/// Date: Apr 2021 Sun 23:43:17

#pragma once

#include <osrpch.h>

#include "GameImporter.h"
#include "CommonImports.h"

void Read(std::ifstream& s, GameFormat_Config& d)
{
    Read(s, d.version);
    Read(s, d.name);
};

void Read(std::ifstream& s, GameFormat_Display& d)
{
    Read(s, d.width);
    Read(s, d.height);
};

void Read(std::ifstream& s, GameFormat_SceneListings& d)
{
    Read(s, d.initialScene);
    Read(s, d.sceneUIDs_cnt);
    for (uint32_t i = 0; i < d.sceneUIDs_cnt; i++)
    {
        Osiris::UID item;
        Read(s, item);
        d.sceneUIDs.push_back(item);
    }
};

