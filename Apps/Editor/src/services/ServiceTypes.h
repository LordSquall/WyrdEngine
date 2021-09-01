#pragma once

#include "core/export.h"


namespace Wyrd::Editor
{
	enum class ServiceType
	{
		Events = 0,
		Workspace = 1,
		Resources = 2,
		Dialog = 3,
		Settings = 4,
		Simulation = 5,
		CoreSystems = 6
	};
}