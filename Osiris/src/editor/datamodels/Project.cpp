#pragma once
#include "osrpch.h"

/* Local includes */
#include "Project.h"

namespace Osiris::Editor
{
	Project::Project() : name("Untitled"), initialScene("initialScene") { }

	Project::Project(std::string name) : name(name), initialScene("initialScene") { }
}