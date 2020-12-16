#pragma once

/* local project */
#include "osrpch.h"
#include "export.h"

namespace Osiris {

	class Shader;
	class Texture;
	class ScriptedClass;

	/**
	 * @brief Resource SubSystem
	 * This class is responsible for storing and maintaining all the resources within the engine, 
	 * and provider a common interface to retrieve each resources type
	*/
	class OSR_LIBRARY_API Resources
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
		std::map<std::string, std::shared_ptr<Texture>> Textures;

		/**
		 * @brief Scripted Class resource map
		*/
		std::map<std::string, std::shared_ptr<ScriptedClass>> Scripts;
	public:
		static Resources* _Instance;
	};
}