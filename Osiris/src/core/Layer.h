#pragma once

#include "core/export.h"
#include "events/Event.h"

#include "core/renderer/Renderer.h"

namespace Osiris {

	class  Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnRender(Renderer& renderer) {}
		virtual void OnEvent(Event& event) {}

		/* Getters and Setters */
		inline const std::string& GetName() const { return _Name; }

		inline bool IsEnabled() { return _Enabled; }
		inline void SetEnabled(const bool flag) { _Enabled = flag; }

	protected:
		bool _Enabled;
		std::string _Name;
	};
}