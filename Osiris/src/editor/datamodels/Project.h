#pragma once

#include <Osiris.h>

#include "AssetManager.h"

using namespace Osiris;

namespace Osiris::Editor {

	class Project
	{
	public:
		Project();
		Project(std::string name);
		~Project() {}
		
		std::string name;
		std::string initialScene;

		inline const AssetManager& GetAssetManager() const { return _AssetManager; }

	private:
		AssetManager _AssetManager;
	};
}