#pragma once

/* local includes */
#include "core/scene/SceneLayer.h"

namespace Osiris {

	class OSR_LIBRARY_API SceneLayer3D : public SceneLayer
	{
	public:
		/* Constructors */
		SceneLayer3D(std::string name) : SceneLayer(name) {}
		~SceneLayer3D() {}

		/**
		 * @brief See SceneLayer::Render()
		*/
		virtual void Render(Renderer& renderer, const glm::mat4& viewProjectionMat) override;

		/**
		 * @brief See SceneLayer::ToJson()
		*/
		virtual jsonxx::Object ToJson();

		/**
		 * @brief See SceneLayer::FromJson()
		*/
		virtual bool FromJson(jsonxx::Object& object);
	};
}