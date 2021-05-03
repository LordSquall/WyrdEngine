#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/UID.h"
#include "core/Structures.h"

/* external includes */
#include <glm/glm.hpp>
#include <mono/metadata/object-forward.h>

namespace Wyrd
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