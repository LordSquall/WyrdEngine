#pragma once

#include "osrpch.h"

#include "IconLibrary.h"

#include "core/Application.h"

#include <nlohmann/json.hpp>
#include <SOIL.h>

using namespace nlohmann;

namespace Osiris
{
	IconLibrary::IconLibrary()
	{
	}

	IconLibrary::~IconLibrary()
	{

	}


	bool IconLibrary::AddIconsFromFile(std::string& filepath)
	{
		return true;
	}

}