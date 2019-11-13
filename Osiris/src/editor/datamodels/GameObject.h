#pragma once

#include <Osiris.h>

#include "components/Transform2DComponent.h"
#include "components/Transform3DComponent.h"
#include "components/SpriteComponent.h"

using namespace Osiris;

namespace Osiris::Editor {

	class GameObject
	{
	public:
		GameObject();
		GameObject(std::string name);
		GameObject(const GameObject& obj);
		~GameObject();

		std::string name;

		std::shared_ptr<Transform2DComponent> transform2d;
		std::shared_ptr<Transform3DComponent> transform3d;

		std::shared_ptr<SpriteComponent> spriteRender;
	};

}