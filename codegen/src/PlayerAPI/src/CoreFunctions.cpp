/// Auto Generated
/// Date: May 2021 Wed 21:51:31

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

