#pragma once

/* local includes */
#include "core/export.h"
#include "core/Layer.h"

/* external includes */
#include <glm/gtx/transform.hpp>

using namespace Osiris;

namespace Osiris
{
	class SpriteLayer;
	class Shader;

	class OSR_LIBRARY_API Renderer2DLayer : public Layer
	{
	public:
		Renderer2DLayer(std::string name);

		bool OnAttach() override;

		void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		
		void OnRender(Timestep ts, Renderer& renderer) override;

		inline std::vector<SpriteLayer*> GetSpriteLayers() { return _SpriteLayers; }

		SpriteLayer* AddSpriteLayer(std::string name);

		void RemoveSpriteLayer(uint32_t index);
		void RemoveAllSpriteLayers();

	protected:
		std::shared_ptr<Shader> _Shader;

		std::vector<SpriteLayer*> _SpriteLayers;

		glm::mat4 _vpMat;
	};
}