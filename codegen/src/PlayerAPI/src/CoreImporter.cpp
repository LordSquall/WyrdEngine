/// Auto Generated
/// Date: Apr 2021 Sun 23:43:17

#pragma once

#include <osrpch.h>

#include "CoreImporter.h"
#include "CommonImports.h"

void Read(std::ifstream& s, CoreFormat_Config& d)
{
    Read(s, d.name);
    Read(s, d.version);
};

