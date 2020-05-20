#pragma once

/* Core includes */
#include <Osiris.h>

/* Local includes */
#include "GameObject.h"

using namespace Osiris;

namespace Osiris::Editor {

	/* Editor Layer 2D Data Model Structure */
	class Layer2D
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
		void SwapSprite(int a, int b);

	public:
		std::string name;
		std::vector<std::shared_ptr<GameObject>> gameobjects;
	};

}