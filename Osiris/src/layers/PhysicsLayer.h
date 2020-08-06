#pragma once

/* local includes */
#include "Osiris.h"
#include "core/export.h"
#include "core/renderer/Shader.h"
#include "core/pipeline/SpriteLayer.h"
#include "core/pipeline/SpriteBatch.h"

/* external includes */
#include <glm/gtx/transform.hpp>

namespace Osiris::Layers
{
	class OSR_LIBRARY_API PhysicsLayer : public Layer
	{
	public:
		PhysicsLayer();

		bool OnAttach() override;
		void OnDetach() override;
		void OnUpdate(Timestep ts) override;
		void OnRender(Timestep ts, Renderer& renderer) override;
	};
}