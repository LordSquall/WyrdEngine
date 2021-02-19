#pragma once

/* local includes */
#include <core/scene/GameObject.h>

/* external includes */

using namespace Osiris;

namespace Osiris {

	class SpriteComponent;
	class SpriteBatch;

	/* Editor Layer 2D Data Model Structure */
	class OSR_LIBRARY_API Layer2D : public GameObject
	{
	public:
		/* Constructors */
		Layer2D();
		Layer2D(const std::string& name);
		Layer2D(const Layer2D& obj);
		virtual ~Layer2D();

		bool Initialise();
		void Update();
		void Render(Renderer& renderer, const glm::mat4& viewProjectionMat);

		bool RegisterSprite(std::shared_ptr<SpriteComponent> spriteComponent);

		inline SpriteBatch& GetSpriteBatch() const { return *_SpriteBatch; }

		void AssignScripts(Behaviour* behaviour);

	private:
		std::shared_ptr<SpriteBatch> _SpriteBatch;
	};

}