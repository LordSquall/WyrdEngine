#pragma once

/* local includes */
#include "osrpch.h"
#include "core/scene/SceneLayer.h"
#include "core/pipeline/SpriteBatch.h"

namespace Osiris {

	/* forward declarations */
	class SpriteComponent;

	class OSR_LIBRARY_API SceneLayer2D : public SceneLayer
	{
	public:
		/* Constructors */
		SceneLayer2D(std::string name) : SceneLayer(name) {}
		~SceneLayer2D() {}

		/**
		 *	@brief Initailise the Layer2D internal resources
		*/
		bool Initialise();

		/**
		 * @brief Add a game object to the layer. 
		 * Note: this will take ownership of the gameobject
		*/
		inline void AddGameObject(std::unique_ptr<GameObject> gameObject) { _GameObjects.push_back(std::move(gameObject)); }

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

	private:
		std::unique_ptr<SpriteBatch> _SpriteBatch;
	};
}