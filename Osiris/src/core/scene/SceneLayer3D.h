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
		 *	@brief Initailise the Layer2D internal resources
		*/
		virtual bool Initialise() override;

		/**
		 * @brief See SceneLayer::Render()
		*/
		virtual void Render(Renderer& renderer, const glm::mat4& viewProjectionMat) override;

		/**
		 * @brief See GameObject::AssignScripts()
		*/
		virtual void AssignScripts(Behaviour* behaviour) override;

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