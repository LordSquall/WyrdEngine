#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"

namespace Wyrd
{
	class WYRD_LIBRARY_API ISceneManager
	{
	public:
		virtual bool ChangeScene(std::string& sceneName) = 0;
	};
}