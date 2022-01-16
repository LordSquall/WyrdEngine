#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/export.h>
#include <core/Structures.h>

#include <glm/glm.hpp>

namespace Wyrd
{
	/**
	 * @brief Primitive Utils class
	 *
	 * Provides a number of static functions which handle primitive generation
	*/
	class PrimitiveUtils 
	{
	public:
		PrimitiveUtils() = delete;
		~PrimitiveUtils() = default;

		static uint32_t GenerateRect(std::vector<Vertex2D>& vertices, uint32_t offset, glm::vec2 position, glm::vec2 size, Color color, float thickness, float rotationAngle = 0.0f, glm::vec2 rotationOrigin = { 0.0f, 0.0f });
		static uint32_t GenerateLine(std::vector<Vertex2D>& vertices, uint32_t offset, glm::vec2 pt1, glm::vec2 pt2, Color color, float thickness, float rotationAngle = 0.0f, glm::vec2 rotationOrigin = { 0.0f, 0.0f });
		static uint32_t GenerateVector2D(std::vector<Vertex2D>& vertices, uint32_t offset, glm::vec2 pt1, float angle, float length, Color color, float thickness, float rotationAngle = 0.0f, glm::vec2 rotationOrigin = { 0.0f, 0.0f });
		static uint32_t GenerateCircle(std::vector<Vertex2D>& vertices, uint32_t offset, glm::vec2 center, float radius, int resolution, Color color, float thickness, float rotationAngle = 0.0f, glm::vec2 rotationOrigin = { 0.0f, 0.0f });
	};


}