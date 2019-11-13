#pragma once
#include "osrpch.h"

#include "Project.h"

namespace Osiris::Editor
{
	Project::Project() : name("Untitled")
	{

	}

	Project::Project(std::string name) : name(name)
	{

	}
}