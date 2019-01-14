#pragma once

#include "core/export.h"
#include "events/Event.h"

#include "core/Renderer.h"

namespace Osiris {

	class OSIRIS_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnRender(std::shared_ptr<Renderer> renderer) {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}