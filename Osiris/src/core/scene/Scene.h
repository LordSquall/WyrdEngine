#pragma once

/* local includes */
#include "Osiris.h"

namespace Osiris {

	class Layer2D;

	/* Editor Scene Data Model Structure */
	class OSR_LIBRARY_API Scene
	{
	public:
		/* Constructors */
		Scene();
		Scene(std::string name);
		~Scene() {}

		void Update();

	public:
		std::string name;
		std::vector<std::shared_ptr<Layer2D>> layers2D;
		glm::vec4 bgcolor;

		glm::vec3 cameraPosition = { 0.0f, 0.0f, 0.0f };
		float cameraZoom = 0.0f;
	};
}