#pragma once

#include <osrpch.h>
#include <core/Layer.h>
#include <core/Renderer.h>

#include <graphics/Shader.h>

namespace Osiris {

	class OSIRIS_API Resources
	{
	public:
		Resources();

		inline static Resources& Get() { return *s_Instance; };

		void SetRenderer(std::shared_ptr<Osiris::Renderer> renderer);

		bool AddShader(std::shared_ptr<Osiris::Shader> shader);

	public:
		static Resources* s_Instance;

		std::shared_ptr<Osiris::Renderer> m_Renderer;

		std::map<std::string, Osiris::Shader> m_ShaderMap;

	};
}