/// Auto Generated
/// Date: May 2021 Sun 22:56:12

#pragma once

#include <wyrdpch.h>

#include "CommonFunctions.h"

#include "GameFormat.h"

void Read(std::ifstream& s, GameFormat_Config& d)
{
    Read(s, d.version);
    Read(s, d.name);
};

void Write(std::ofstream& s, GameFormat_Config& d)
{
    Write(s, d.version);
    Write(s, d.name);
};

void Read(std::ifstream& s, GameFormat_Display& d)
{
    Read(s, d.width);
    Read(s, d.height);
};

void Write(std::ofstream& s, GameFormat_Display& d)
{
    Write(s, d.width);
    Write(s, d.height);
};

void Read(std::ifstream& s, GameFormat_SceneListings& d)
{
    Read(s, d.initialScene);
    Read(s, d.sceneUIDs_cnt);
    for (uint32_t i = 0; i < d.sceneUIDs_cnt; i++)
    {
        Wyrd::UID item;
        Read(s, item);
        d.sceneUIDs.push_back(item);
    }
};

void Write(std::ofstream& s, GameFormat_SceneListings& d)
{
    Write(s, d.initialScene);
    Write(s, d.sceneUIDs_cnt);
    for (uint32_t i = 0; i < d.sceneUIDs_cnt; i++)
    {
        Write(s, d.sceneUIDs[i]);
    }
};

