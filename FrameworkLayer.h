#pragma once

#include "core/export.h"
#include "core/Layer.h"

namespace WyrdHeadless
{
	class FrameworkLayer : public Layer
	{
	public:
		FrameworkLayer();
		~FrameworkLayer();

		bool OnAttach() override;
		void OnDetach() override;
		void OnUpdate(Timestep ts) override;
		void OnRender(Timestep ts, Renderer& renderer) override;
		void OnEvent(Event& event) override;
	};
}