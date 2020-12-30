#pragma once

/* core osiris includes */
#include "osrpch.h"

struct CompileResults
{
	bool success;
	std::vector<std::string> errors;
	std::vector<std::string> warnings;
};