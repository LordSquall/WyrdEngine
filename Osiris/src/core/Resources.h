#pragma once

/* local project */
#include "osrpch.h"
#include "core/Layer.h"
#include "core/renderer/Shader.h"
#include "core/renderer/Texture.h"
#include "core/behaviour/ScriptedClass.h"
#include "core/behaviour/ScriptedGameObject.h"

namespace Osiris {

	class OSR_LIBRARY_API Resources
	{
	public:
		Resources();

		inline static Resources& Get() { return *_Instance; };

	public:
		std::map<std::string, std::shared_ptr<Shader>> Shaders;
		std::map<std::string, std::shared_ptr<Texture>> Textures;
		std::map<std::string, std::shared_ptr<ScriptedClass>> Scripts;
	public:
		static Resources* _Instance;
	};
}