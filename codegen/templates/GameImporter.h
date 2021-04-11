/// Auto Generated
/// Date: Apr 2021 Sun 10:20:05

#pragma once

#include <osrpch.h>

class GameFormatImporter
{
    static void Read(std::ifstream& s, GameFormat_Config& d);
    static void Read(std::ifstream& s, GameFormat_Display& d);
    static void Read(std::ifstream& s, GameFormat_SceneListings& d);
};
