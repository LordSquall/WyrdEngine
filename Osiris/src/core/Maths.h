#pragma once

#include "core/export.h"
#include "osrpch.h"

namespace Osiris
{
	class Maths
	{
	public:
		static float RandomFloat(float min, float max)
		{
			float random = ((float)rand()) / (float)RAND_MAX;
			float range = max - min;
			return (random*range) + min;
		}
	};


}