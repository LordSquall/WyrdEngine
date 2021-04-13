/// Auto Generated
/// Date: Apr 2021 Tue 22:10:41

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

