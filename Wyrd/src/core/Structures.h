#pragma once

#include "core/export.h"
#include "events/Event.h"

#include "core/renderer/Renderer.h"
#include "core/Timestep.h"

namespace Wyrd {

	struct Color
	{
		float r;
		float g;
		float b;
		float a;
	};

	struct Vector2
	{
		float x;
		float y;
	};

	struct Vector3
	{
		float x;
		float y;
		float z;
	};
}