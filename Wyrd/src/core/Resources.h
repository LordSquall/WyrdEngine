#pragma once

/* local includes */
#include "wyrdpch.h"
#include "export.h"
#include "core/UID.h"

/* external includes */

/* default resource UID */
#define RESOURCE_DEFAULT_TEXTURE	"f7ad951c-50a4-43b2-8b5f-88c484691f11"

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