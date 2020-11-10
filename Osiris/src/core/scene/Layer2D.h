#pragma once

/* local includes */
#include <core/scene/GameObject.h>

using namespace Osiris;

namespace Osiris {

	/* Editor Layer 2D Data Model Structure */
	class OSR_LIBRARY_API Layer2D : public GameObject
	{
	public:
		/* Constructors */
		Layer2D();
		Layer2D(const std::string& name);
		Layer2D(const Layer2D& obj);
		virtual ~Layer2D();

		void Update();
	};

}