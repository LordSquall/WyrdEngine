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

		inline const std::string& GetName() const { return _Name; }

		inline bool Enabled() { return _IsEnabled; }
		inline void Enabled(const bool flag) { _IsEnabled = flag; }

	protected:
		bool _IsEnabled;
		std::string _Name;
	};
}