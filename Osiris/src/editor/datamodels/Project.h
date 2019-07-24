#pragma once

#include <Osiris.h>

using namespace Osiris;


namespace Osiris::Editor {

	class Project
	{
	public:
		Project() : _Name("untitled") {}
		Project(std::string name) : _Name(name) {}
		~Project() {}
		
		inline const std::string GetName() const { return _Name; }
		inline void SetName(const std::string name) { _Name = name; }

	private:
		std::string _Name;
	};
}