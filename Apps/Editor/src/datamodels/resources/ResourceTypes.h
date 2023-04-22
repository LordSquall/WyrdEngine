#pragma once

/* core wyrd includes */
#include <wyrdpch.h>

namespace Wyrd::Editor
{
	enum ResourceType
	{
		NONE = 0,
		UNKNOWN = 1,
		TEXTURE = 2,
		SHADER = 3,
		MODEL = 4,
		SCENE = 5,
		SCRIPT = 6,
		FONT = 7,
		MATERIAL = 8
	};
}