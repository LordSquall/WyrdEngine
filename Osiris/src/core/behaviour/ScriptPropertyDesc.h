#pragma once

/* local includes */
#include "osrpch.h"
#include "core/UID.h"
#include "core/Structures.h"

/* external includes */
#include <glm/glm.hpp>

namespace Osiris
{
	/**
	 * @brief Description of a runtime property of the script
	*/
	struct ScriptPropertyDesc
	{
		float floatVal;
		int intVal;
		std::string stringVal;
		UID objectUID;
		std::string objectNameVal;
		std::string objectClassNameVal;
		void* objectVal;
		Color colorVal;
	};
};