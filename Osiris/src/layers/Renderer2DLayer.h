#pragma once

#include <Osiris.h>

#include "core/renderer/Shader.h"
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

	private:
		std::unique_ptr<Shader> _Shader;

		std::unique_ptr<SpriteBatch> _staticSpriteBatch;

		glm::mat4 _vpMat;
	};
}