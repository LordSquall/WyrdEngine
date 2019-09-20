#pragma once

#include <Osiris.h>

using namespace Osiris;


namespace Osiris::Editor {

	class GameObject2D
	{
	public:
		std::string name;
	};

	class Layer2D
	{
	public:
		std::string name;
		std::vector<std::shared_ptr<GameObject2D>> sprites;

		void AddSprite(const GameObject2D& gameObject);
		void RemoveSprite(int uid);
		void SwapSprite(int a, int b);
	};

	class Scene
	{
	public:
		Scene();

		Scene(std::string name);

		~Scene() {}

		void BuildDefaults();

		std::string name;
		std::vector<std::shared_ptr<Layer2D>> layers2D;
	};
}