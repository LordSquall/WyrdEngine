#pragma once

#include "core/export.h"
#include "core/defines.h"

#include <glm/glm.hpp>

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

		Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f){}
		Color(float _r, float _g, float _b) : r(_r), g(_g), b(_b), a(1.0f) {}
		Color(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a){}
	};

	struct Vector2
	{
		float x;
		float y;
	
		Vector2() : x(0.0f), y(0.0f) {}
		Vector2(float _x, float _y) : x(_x), y(_y) {}

		Vector2& operator= (const glm::vec2& o)
		{
			this->x = o.x;
			this->y = o.y;
			return *this;
		}

		Vector2& operator+ (const Vector2& o)
		{
			this->x += o.x;
			this->y += o.y;
			return *this;
		}

		Vector2& operator- (const Vector2& o)
		{
			this->x -= o.x;
			this->y -= o.y;
			return *this;
		}
	
		static Vector2 RotateAtPoint(Vector2 pt, float angle, Vector2 pivot)
		{
			float s = sin(DEG_TO_RADF(angle));
			float c = cos(DEG_TO_RADF(angle));
	
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
	
		Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
		Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

		Vector3& operator= (const glm::vec3& o)
		{
			this->x = o.x;
			this->y = o.y;
			this->z = o.z;
			return *this;
		}

		Vector3& operator+ (const glm::vec3& o)
		{
			this->x += o.x;
			this->y += o.y;
			this->z += o.z;
			return *this;
		}
	};

	struct ImageArea
	{
		int32_t id;
		std::string name;
		Vector2 uv1;		// bottom left
		Vector2 uv2;		// top right
	};
}