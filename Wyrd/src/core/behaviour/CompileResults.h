#pragma once

/* core wyrd includes */
#include "wyrdpch.h"

struct CompileResults
{
	bool success;
	std::vector<std::string> errors;
	std::vector<std::string> warnings;
};