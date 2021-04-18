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
		virtual bool Initialise() override;

		/**
		 * @brief Add a game object to the layer. 
		 * Note: this will take ownership of the gameobject
		*/
		virtual GameObject* AddGameObject(std::unique_ptr<GameObject> gameObject);

		/**
		 * @brief See SceneLayer::Render()
		*/
		virtual void Render(Renderer& renderer, const glm::mat4& viewProjectionMat) override;

		/**
		* @brief Get the Layers Sprite Batch
		*/
		inline SpriteBatch* GetSpriteBatch() const { return _SpriteBatch.get(); }

		/**
		 * @brief See GameObject::AssignScripts()
		*/
		virtual void AssignScripts(Behaviour* behaviour) override;

		/**
		 * @brief See SceneLayer::ToJson()
		*/
		virtual bool ToJson(jsonxx::Object& object) override;

		/**
		 * @brief See SceneLayer::FromJson()
		*/
		virtual bool FromJson(jsonxx::Object& object) override;

	private:
		std::unique_ptr<SpriteBatch> _SpriteBatch;
	};
}