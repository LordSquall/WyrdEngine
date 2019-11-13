#pragma once

#include <Osiris.h>


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
	};
}