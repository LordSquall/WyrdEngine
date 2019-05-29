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
}