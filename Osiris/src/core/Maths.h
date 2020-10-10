#pragma once

#include "core/export.h"
#include "osrpch.h"

namespace Osiris
{
	/**
	 * @brief Simple Wrapper class for a number of useful standalone maths functions
	*/
	class Maths
	{
	public:
		/**
		 * @brief Generate a 'random' float number within a specific range
		 * @param min 
		 * @param max 
		 * @return float - Random number
		*/
		static float RandomFloat(float min, float max)
		{
			float random = ((float)rand()) / (float)RAND_MAX;
			float range = max - min;
			return (random*range) + min;
		}
	};


}