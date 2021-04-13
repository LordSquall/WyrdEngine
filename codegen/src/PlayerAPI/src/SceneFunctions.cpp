/// Auto Generated
/// Date: Apr 2021 Tue 21:51:40

#pragma once

#include <osrpch.h>

#include "CommonFunctions.h"

#include "SceneFormat.h"

void Read(std::ifstream& s, SceneFormat_Config& d)
{
    Read(s, d.version);
};

void Write(std::ofstream& s, SceneFormat_Config& d)
{
    Write(s, d.version);
};

