#pragma once

/* local includes  */
#include "core/export.h"
#include "core/Timestep.h"
#include "core/scene/Scene.h"

namespace Wyrd {

	struct ISystemContext
	{
		Scene& scene;

		ISystemContext(Scene& scene) : scene(scene) {}
	};

	class WYRD_LIBRARY_API ISystem
	{
	public:
		virtual ~ISystem() {};

		virtual bool OnUpdate(ISystemContext& context) = 0;
	};
}