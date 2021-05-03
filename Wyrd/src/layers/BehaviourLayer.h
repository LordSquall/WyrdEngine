#pragma once

/* local includes */
#include "core/export.h"
#include "core/Layer.h"

namespace Wyrd
{
	class WYRD_LIBRARY_API BehaviourLayer : public Layer
	{
	public:
		BehaviourLayer();
		virtual ~BehaviourLayer() {};

		bool OnAttach() override;
		void OnDetach() override;
		void OnUpdate(Timestep ts) override;
		void OnRender(Timestep ts, Renderer& renderer) override;
	};
}