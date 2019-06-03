#pragma once

#include <osrpch.h>
#include <core/Layer.h>
#include <core/renderer/Renderer.h>


namespace Osiris {

	class  Resources
	{
	public:
		Resources();

		inline static Resources& Get() { return *s_Instance; };

		void SetRenderer(std::shared_ptr<Osiris::Renderer> renderer);

	public:
		static Resources* s_Instance;

		std::shared_ptr<Osiris::Renderer> m_Renderer;

	};
}