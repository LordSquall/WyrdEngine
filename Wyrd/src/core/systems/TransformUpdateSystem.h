#pragma once

/* local includes */
#include "core/export.h"
#include "core/Timestep.h"
#include "core/systems/ISystem.h"

namespace Wyrd {

	struct TransformUpdateSystemContext : public ISystemContext
	{
		TransformUpdateSystemContext(Scene& s) : ISystemContext(s) { };
	};

	struct TransformStackTransition
	{
		Entity from;
		Entity to;
		glm::mat4 mat;
	};

	class WYRD_LIBRARY_API TransformUpdateSystem : public ISystem
	{
	public:
		TransformUpdateSystem();

		virtual bool OnUpdate(ISystemContext& context);

	private:
		std::stack<TransformStackTransition> _MatrixStack;
	};
}