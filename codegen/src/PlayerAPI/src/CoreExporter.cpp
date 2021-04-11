/// Auto Generated
/// Date: Apr 2021 Sun 23:43:17

#pragma once

#include <osrpch.h>

#include "CoreExporter.h"
#include "CommonExports.h"

void Write(std::ofstream& s, CoreFormat_Config& d)
{
    Write(s, d.name);
    Write(s, d.version);
};

