#pragma once

/* local includes */
#include "wyrdpch.h"
#include "export.h"
#include "core/UID.h"
#include "core/ResourcesUIDs.h"

/* external includes */

namespace Wyrd {

	class Shader;
	class Texture;
	class ScriptedClass;
	class FontType;

	/**
	 * @brief Resource SubSystem
	 * This class is responsible for storing and maintaining all the resources within the engine, 
	 * and provider a common interface to retrieve each resources type
	*/
	class WYRD_LIBRARY_API Resources
	{
	public:
		/**
		 * @brief Constructor
		*/
		Resources();

		/**
		 * @brief Singleton access function
		 * @return Resources - Statis instance
		*/
		inline static Resources& Get() { return *_Instance; };

	public:
		/**
		 * @brief Shader resource map 
		*/
		std::map<std::string, std::shared_ptr<Shader>> Shaders;

		/**
		 * @brief Texture resource map
		*/
		std::map<UID, std::shared_ptr<Texture>> Textures;

		/**
		 * @brief FontType resource map
		*/
		std::map<std::string, std::shared_ptr<FontType>> FontTypes;

	public:
		static Resources* _Instance;
	};
}