/// Auto Generated
/// Date: Apr 2021 Sun 21:39:54

#pragma once

#include <osrpch.h>

#include "CommonFunctions.h"

#include "CoreFormat.h"

void Read(std::ifstream& s, CoreFormat_Config& d)
{
    Read(s, d.name);
    Read(s, d.version);
};

void Write(std::ofstream& s, CoreFormat_Config& d)
{
    Write(s, d.name);
    Write(s, d.version);
};

