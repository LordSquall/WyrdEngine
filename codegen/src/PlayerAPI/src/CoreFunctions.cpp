/// Auto Generated
/// Date: May 2021 Sun 22:56:12

#pragma once

#include <wyrdpch.h>

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

