#pragma once

#include "core/export.h"
#include "core/defines.h"

namespace Wyrd {

	struct Color
	{
		float r;
		float g;
		float b;
		float a;

		static Color RED;
		static Color GREEN;
		static Color BLUE;
		static Color CYAN;
		static Color YELLOW;
		static Color MAGENTA;
		static Color BLACK;
		static Color WHITE;
	};

	struct Vector2
	{
		float x;
		float y;

		Vector2() : x(0.0f), y(0.0f) {}
		Vector2(float _x, float _y) : x(_x), y(_y) {}

		static Vector2 RotateAtPoint(Vector2 pt, float angle, Vector2 pivot)
		{
			float s = sin(WYRD_DEG_TO_RAD(angle));
			float c = cos(WYRD_DEG_TO_RAD(angle));

			pt.x -= pivot.x;
			pt.y -= pivot.y;

			// rotate point
			float xnew = pt.x * c - pt.y * s;
			float ynew = pt.x * s + pt.y * c;

			// translate point back:
			pt.x = xnew + pivot.x;
			pt.y = ynew + pivot.y;
			return pt;
		}
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