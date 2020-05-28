#pragma once

/* Core includes */
#include <Osiris.h>

/* Local includes */
#include "components/Transform2DComponent.h"
#include "components/Transform3DComponent.h"
#include "components/SpriteComponent.h"
#include "components/ScriptComponent.h"

using namespace Osiris;

namespace Osiris::Editor {

	/* Editor GameObject Data Model Structure */
	class GameObject
	{
	public:
		/* Constructors */
		GameObject();
		GameObject(std::string name);
		GameObject(const GameObject& obj);
		~GameObject();

		inline uint32_t GetUID() { return _uid; }

	public:
		std::string name;
		Transform2DComponent transform2d;
		SpriteComponent spriteRender;
		ScriptComponent script;

		glm::vec4 inputArea;

	private:
		uint32_t _uid = 0u;
		static uint32_t _nextUid;
	};

}