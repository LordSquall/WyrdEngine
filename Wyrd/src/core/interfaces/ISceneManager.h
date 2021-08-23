#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"

namespace Wyrd
{
	/**
	 * @brief ISceneManager class (interface)
	 *
	 * Behaviour interface for all Scene Management functions.
	*/
	class WYRD_LIBRARY_API ISceneManager
	{
	public:
		/**
		 * @brief Change the scene to a specific scene name
		 * @param sceneName
		*/
		virtual bool ChangeScene(std::string& sceneName) = 0;
	};
}