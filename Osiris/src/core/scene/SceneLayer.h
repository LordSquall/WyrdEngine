#pragma once

/* local includes */
#include "core/export.h"
#include "core/UID.h"
#include "core/Structures.h"
#include "core/scene/GameObject.h"

/* external includes */
#include <glm/glm.hpp>

namespace Osiris {

	class OSR_LIBRARY_API SceneLayer : public GameObject
	{
	public:
		/* Constructors */
		SceneLayer(std::string name) : _Name(name) { _UID = UIDUtils::Create(); }
		SceneLayer(std::string name, UID uid) : _Name(name), _UID(uid) {}
		~SceneLayer() {}

		/**
		 *	@brief Initailise the Layer2D internal resources
		*/
		virtual bool Initialise() = 0;

		/**
		 * @brief Get the layer name
		 * @param current system renderer
		 * @param view projection matrix to use for the render operation
		*/
		virtual void Render(Renderer& renderer, const glm::mat4& viewProjectionMat) = 0;

		/**
		 * @brief Add a gameobject to the layer
		 * @param game object
		*/
		virtual GameObject* AddGameObject(std::unique_ptr<GameObject> gameObject) = 0;

		/**
		 * @brief Get the layer name
		 * @return layer name
		*/
		inline std::string& GetName() { return _Name; }

		/**
		 * @brief Get the layer visible status
		 * @return layer visible
		*/
		inline const bool IsVisible() { return _IsVisible; }

		/**
		 * @brief Set the layer visible status
		*/
		inline void SetVisible(const bool visible) { _IsVisible = visible; }

		/**
		 * @brief Get the layer active status
		 * @return layer active
		*/
		inline const bool IsActive() { return _IsActive; }

		/**
		* @brief Set the layer active status
		*/
		inline void SetActive(const bool active) { _IsActive = active; }

		/**
		 * @brief Get the layer uid
		 * @return layer uid
		*/
		inline const UID& GetUID() { return _UID; }

		/**
		 * @brief See GameObject::AssignScripts()
		*/
		virtual void AssignScripts(Behaviour* behaviour) = 0;

		/**
		 * @brief Serialise the scenelayer into a json object
		 * @return json object
		*/
		virtual bool ToJson(jsonxx::Object& object) = 0;

		/**
		 * @brief Deserialise a json object into a scenelayer
		 * @param object json object
		 * @return true is successful
		*/
		virtual bool FromJson(jsonxx::Object& object) = 0;

	protected:
		std::string _Name;
		UID _UID;
		bool _IsVisible;
		bool _IsActive;
	};
}