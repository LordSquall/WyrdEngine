#pragma once

/* core osiris includes */
#include <osrpch.h>

namespace Osiris::Editor
{
	enum ResourceType
	{
		NONE = 0,
		UNKNOWN = 1,
		TEXTURE = 2,
		SHADER = 3,
		MODEL = 4,
		SCENE = 5,
		SCRIPT = 6
	};
}