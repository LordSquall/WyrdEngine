#pragma once

/* local includes */
#include "osrpch.h"
#include "core/UID.h"

/* external includes */
#include <glm/glm.hpp>

namespace Osiris
{
	enum struct PropType
	{
		INT = 1,
		FLOAT = 2,
		STRING = 3,
		GAMEOBJECT = 4,
		TEXTURE = 5,
		COLOR = 6,
		UNSUPPORTED = 0
	};

	/**
	 * @brief Description of a runtime property of the script
	*/
	struct PropertyDesc
	{
		std::string name;
		void* getter;
		void* setter;
		enum PropType type;

		// Values
		float floatVal;
		int intVal;
		std::string stringVal;
		UID objectVal;
		std::string objectNameVal;
		std::string objectClassNameVal;
		glm::vec4 colorVal;
	};
};