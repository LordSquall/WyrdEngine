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
	};
}