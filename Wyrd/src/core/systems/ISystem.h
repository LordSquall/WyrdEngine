#pragma once

/* local includes  */
#include "core/export.h"
#include "core/Timestep.h"

namespace Wyrd {

	class WYRD_LIBRARY_API ISystem
	{
	public:
		virtual ~ISystem() {};

		virtual bool OnUpdate() = 0;
	};
}