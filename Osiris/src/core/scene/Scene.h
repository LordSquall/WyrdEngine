#pragma once

/* local includes */
#include "core/export.h"
#include "core/UID.h"
#include "core/Structures.h"
#include "core/scene/SceneLayer.h"

#include <glm/glm.hpp>

namespace Osiris {

	class Layer2D;
	class GameObject;
	class Behaviour;

	/* Editor Scene Data Model Structure */
	class OSR_LIBRARY_API Scene
	{
	public:
		/* Constructors */
		Scene();
		Scene(std::string name);
		~Scene() {}

		void Update();

		inline std::vector<std::unique_ptr<SceneLayer>>& GetLayers() { return _Layers; }

		GameObject* FindGameObject(const UID uid);

		void AssignScripts(Behaviour* behaviour);

		void AddLayer(std::unique_ptr<SceneLayer> layer);
		void RemoveLayer(const UID& uid);

		/**
		 * @brief Serialise the scene into a json object
		 * @return json object
		*/
		jsonxx::Object ToJson();

		/**
		 * @brief Deserialise a json object into a scene
		 * @param object json object
		 * @return true is successful
		*/
		bool FromJson(jsonxx::Object& object);

	public:
		std::string name;
		//std::vector<std::shared_ptr<Layer2D>> layers2D;
		Color bgcolor;

		glm::vec3 cameraPosition = { 0.0f, 0.0f, 0.0f };
		float cameraZoom = 0.0f;

	private:
		std::vector<std::unique_ptr<SceneLayer>> _Layers;
	};
}