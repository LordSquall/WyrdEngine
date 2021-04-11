/// Auto Generated
/// Date: Apr 2021 Sun 10:20:05

#pragma once

#include <osrpch.h>

class GameFormatExporter
{
    static void Write(std::ofstream& s, GameFormat_Config& d);
    static void Write(std::ofstream& s, GameFormat_Display& d);
    static void Write(std::ofstream& s, GameFormat_SceneListings& d);
};
