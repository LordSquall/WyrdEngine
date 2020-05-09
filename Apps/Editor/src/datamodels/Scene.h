#pragma once

/* Core includes */
#include <Osiris.h>
#include <core/pipeline/SpriteBatch.h>

/* Local includes */
#include "Layer2D.h"

using namespace Osiris;

namespace Osiris::Editor {

	/* Editor Scene Data Model Structure */
	class Scene
	{
	public:
		/* Constructors */
		Scene();
		Scene(std::string name);
		~Scene() {}

	public:
		std::string name;
		std::vector<std::shared_ptr<Layer2D>> layers2D;
		float bgcolor[3];

		glm::vec3 cameraPosition = { 0.0f, 0.0f, 0.0f };
		float cameraZoom = 0.0f;
	};
}