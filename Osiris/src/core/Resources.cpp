#include "osrpch.h"

#include "Resources.h"

namespace Osiris {

	Resources* Resources::s_Instance = nullptr;

	Resources::Resources()
	{
		OSR_CORE_ASSERT(!s_Instance, "Resources in not null!");

		s_Instance = this;
	}

	void Resources::SetRenderer(std::shared_ptr<Osiris::Renderer> renderer)
	{
		m_Renderer = renderer;
	}

	bool Resources::AddShader(std::shared_ptr<Osiris::Shader> shader)
	{
		if (m_Renderer->LoadShader(shader) == 0)
		{
			OSR_CLIENT_ERROR("Unable to load Shader!");
			return false;
		}

		m_ShaderMap.insert(std::pair<std::string, Osiris::Shader>("Shader 1", *shader));

		return true;
	}
}