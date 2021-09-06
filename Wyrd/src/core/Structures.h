#pragma once

#include "core/export.h"


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

	struct ImageArea
	{
		int32_t id;
		std::string name;
		Vector2 uv1;		// bottom left
		Vector2 uv2;		// top right
	};
}