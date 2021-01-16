#pragma once

/* core osiris includes */
#include <osrpch.h>

/* local includes */
#include "UUIDUtils.h"

namespace Osiris::Editor
{
	UUID UUIDUtils::Create()
	{
		return xg::newGuid();
	}
}