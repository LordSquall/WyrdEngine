#pragma once

#include "core/export.h"
#include "events/Event.h"

#include "core/renderer/Renderer.h"
#include "core/Timestep.h"

namespace Osiris {

	class  Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual bool OnAttach() { return true; }
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnRender(Timestep ts, Renderer& renderer) {}
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