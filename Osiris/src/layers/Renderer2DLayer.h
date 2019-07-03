#pragma once

#include <Osiris.h>

#include "core/renderer/Shader.h"
#include "core/pipeline/SpriteLayer.h"
#include "core/pipeline/SpriteBatch.h"

#include <glm/gtx/transform.hpp>

using namespace Osiris;

namespace Osiris::Layers
{
	class Renderer2DLayer : public Layer
	{
	public:
		Renderer2DLayer(std::string name);

		void OnAttach() override;

		void OnDetach() override;

		void OnUpdate() override;

		void OnEvent(Event& event) override;

		void OnRender(Renderer& renderer) override;

		inline std::vector<SpriteLayer*> GetSpriteLayers() { return _SpriteLayers; }

		void AddSpriteLayer(std::string name);
		void RemoveSpriteLayer(uint32_t index);

	private:
		std::unique_ptr<Shader> _Shader;

		std::vector<SpriteLayer*> _SpriteLayers;

		glm::mat4 _vpMat;
	};
}