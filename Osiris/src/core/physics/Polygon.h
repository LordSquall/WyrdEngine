#pragma once

#include "osrpch.h"

#include <glm/glm.hpp>

namespace Osiris
{
	class Polygon
	{
	public:
		enum Direction
		{
			NONE,
			NORTH,
			EAST,
			SOUTH,
			WEST
		};

	public:
		Polygon();
		virtual ~Polygon();

	public:
		void BuildFromRectangle(const glm::vec4 rectangle);

		static Direction CollisionTest_AABB(Polygon* polygonA, Polygon* polygonB);

		static bool CollisionTest_SAT(Polygon* polygonA, Polygon* polygonB, glm::vec2* collisionNormal, float* depth);

		static bool CollisionTest_DIAG(Polygon* polygonA, Polygon* polygonB, glm::vec2* collisionNormal, float* depth);

		glm::vec2 center;
		glm::vec4 aabb;
		std::vector<glm::vec2> pts;
	};
}