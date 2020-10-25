#pragma once

/* local includes */
#include <core/scene/GameObject.h>

using namespace Osiris;

namespace Osiris {

	class GameObject;

	/* Editor Layer 2D Data Model Structure */
	class OSR_LIBRARY_API Layer2D
	{
	public:
		/* Constructors */
		Layer2D();
		Layer2D(const std::string& name);
		Layer2D(const Layer2D& obj);
		virtual ~Layer2D();

	public:
		void AddSprite(std::shared_ptr<GameObject> gameObject);
		void RemoveSprite(int uid);
		void DuplicateSprite(int uid);
		void SwapSprite(int a, int b);

		void Update();
	public:
		std::string name;
		std::vector<std::shared_ptr<GameObject>> gameobjects;
	};

}