#pragma once

#include <osrpch.h>
#include <core/Layer.h>
#include <core/Renderer.h>

#include <graphics/Shader.h>
#include <graphics/Texture.h>

namespace Osiris {

	class OSIRIS_API Resources
	{
	public:
		Resources();

		inline static Resources& Get() { return *s_Instance; };

		void SetRenderer(std::shared_ptr<Osiris::Renderer> renderer);

		bool AddShader(std::shared_ptr<Osiris::Shader> shader);

		bool AddTexture(std::shared_ptr<Osiris::Texture> texture);

	public:
		static Resources* s_Instance;

		std::shared_ptr<Osiris::Renderer> m_Renderer;

		std::vector<std::shared_ptr<Osiris::Shader>> m_ShaderMap;
		std::vector<std::shared_ptr<Osiris::Texture>> m_TextureMap;

	};
}