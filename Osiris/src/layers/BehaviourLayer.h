#pragma once

/* local includes */
#include "core/export.h"
#include "core/Layer.h"

namespace Osiris
{
	class OSR_LIBRARY_API BehaviourLayer : public Layer
	{
	public:
		BehaviourLayer();

		bool OnAttach() override;
		void OnDetach() override;
		void OnUpdate(Timestep ts) override;
		void OnRender(Timestep ts, Renderer& renderer) override;
	};
}