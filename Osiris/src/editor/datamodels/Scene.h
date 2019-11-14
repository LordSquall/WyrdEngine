#pragma once

#include <Osiris.h>
#include <core/pipeline/SpriteBatch.h>

#include "Layer2D.h"

using namespace Osiris;

namespace Osiris::Editor {

	class Scene
	{
	public:
		Scene();

		Scene(std::string name);

		~Scene() {}

		void BuildDefaults();

		std::string name;
		std::vector<std::shared_ptr<Layer2D>> layers2D;
		float bgcolor[3];
	};
}