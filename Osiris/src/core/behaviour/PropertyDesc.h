#pragma once

/* local includes */
#include "osrpch.h"
#include "core/UID.h"
#include "core/Structures.h"

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
		UID objectUID;
		std::string objectNameVal;
		std::string objectClassNameVal;
		void* objectVal;
		Color colorVal;
	};

	/**
	 * @brief Description of a runtime function of the script
	*/
	struct FunctionDesc
	{
		std::string name;
		void* func;
	};
};