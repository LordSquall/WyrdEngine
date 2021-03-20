#pragma once

/* local includes */
#include "core/export.h"
#include "core/UID.h"
#include "core/Structures.h"
#include "core/scene/GameObject.h"

/* external includes */
#include <glm/glm.hpp>
#include <jsonxx.h>

namespace Osiris {

	class OSR_LIBRARY_API SceneLayer : public GameObject
	{
	public:
		/* Constructors */
		SceneLayer(std::string name) : _Name(name) { _UID = UIDUtils::Create(); }
		SceneLayer(std::string name, UID uid) : _Name(name), _UID(uid) {}
		~SceneLayer() {}

		/**
		 * @brief Get the layer name
		 * @param current system renderer
		 * @param view projection matrix to use for the render operation
		*/
		virtual void Render(Renderer& renderer, const glm::mat4& viewProjectionMat) = 0;

		/**
		 * @brief Get the layer name
		 * @return layer name
		*/
		inline const std::string& GetName() { return _Name; }

		/**
		 * @brief Get the layer uid
		 * @return layer uid
		*/
		inline const UID& GetUID() { return _UID; }

		/**
		 * @brief Serialise the scenelayer into a json object
		 * @return json object
		*/
		virtual jsonxx::Object ToJson() = 0;

		/**
		 * @brief Deserialise a json object into a scenelayer
		 * @param object json object
		 * @return true is successful
		*/
		virtual bool FromJson(jsonxx::Object& object) = 0;

	protected:
		std::string _Name;
		UID _UID;
	};
}