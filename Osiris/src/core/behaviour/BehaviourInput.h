#pragma once

/* local includes */
#include "osrpch.h"
#include "core/export.h"
#include "core/Structures.h"

namespace Osiris
{

	/**
	 * @brief Behaviour Input class
	 *
	 * This class is a storage class for all input which needs to be passed to the VM
	*/
	class OSR_LIBRARY_API BehaviourInput
	{
	public:
		Vector2 MouseInput;
		bool MouseButtons[2];
	};
}