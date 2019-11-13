#pragma once

#include <Osiris.h>
#include <core/pipeline/SpriteBatch.h>

#include "GameObject.h"

using namespace Osiris;

namespace Osiris::Editor {

	class Layer2D
	{
	public:
		Layer2D();
		Layer2D(const std::string& name);
		Layer2D(const Layer2D& obj);
		virtual ~Layer2D();

		std::string name;
		std::vector<std::shared_ptr<GameObject>> gameObjects;

		void Render(Renderer& renderer);
		void AddSprite(const GameObject& gameObject);
		void RemoveSprite(int uid);
		void SwapSprite(int a, int b);

	};

}