#pragma once

/* Core includes */
#include <Osiris.h>

using namespace Osiris;

namespace Osiris::Editor {

	/* Editor Project Data Model Structure */
	class Project
	{
	public:
		/* Constructors */
		Project();
		Project(std::string name);
		~Project() {}
		
	public:
		std::string name;
		std::string initialScene;
	};
}