#pragma once

#include <Osiris.h>

using namespace Osiris;


namespace Osiris {

	class Scene
	{
	public:
		Scene() : _ID(_nextID), _Name("untitled")
		{
			_nextID = _nextID + 1;
		}

		Scene(std::string name) : _ID(_nextID), _Name(name)
		{
			_nextID = _nextID + 1;
		}

		~Scene() {}

		void Unload();

		/* Getters and Setters */
		inline const uint32_t GetID() const { return _ID; }

		inline const std::string GetName() const { return _Name; }
		inline void SetName(const std::string name) { _Name = name; }
		


	private:
		uint32_t _ID;
		std::string _Name;

		static uint32_t _nextID;
	};
}