#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/Application.h"
#include "core/Log.h"
#include "core/support/PrimitiveUtils.h"


namespace Wyrd {

	uint32_t PrimitiveUtils::GenerateRect(std::vector<Vertex2D>& vertices, uint32_t offset, glm::vec2 position, glm::vec2 size, Color color, float thickness, float rotationAngle, glm::vec2 rotationOrigin)
	{
		float t = thickness * 0.5f;

		// top
		vertices.push_back({ position.x,				position.y + size.y + t,	color.r, color.g, color.b, color.a });	// top right
		vertices.push_back({ position.x,				position.y + size.y - t,	color.r, color.g, color.b, color.a });	// bottom right
		vertices.push_back({ position.x + size.x,		position.y + size.y - t,	color.r, color.g, color.b, color.a });	// bottom left
		vertices.push_back({ position.x + size.x,		position.y + size.y - t,	color.r, color.g, color.b, color.a });	// bottom left
		vertices.push_back({ position.x + size.x,		position.y + size.y + t,	color.r, color.g, color.b, color.a });	// top left 
		vertices.push_back({ position.x,				position.y + size.y + t,	color.r, color.g, color.b, color.a });	// top right
		// bottom													 
		vertices.push_back({ position.x,				position.y + t,				color.r, color.g, color.b, color.a });	// top right
		vertices.push_back({ position.x,				position.y - t,				color.r, color.g, color.b, color.a });	// bottom right
		vertices.push_back({ position.x + size.x,		position.y - t,				color.r, color.g, color.b, color.a });	// bottom left
		vertices.push_back({ position.x + size.x,		position.y - t,				color.r, color.g, color.b, color.a });	// bottom left
		vertices.push_back({ position.x + size.x,		position.y + t,				color.r, color.g, color.b, color.a });	// top left 
		vertices.push_back({ position.x,				position.y + t,				color.r, color.g, color.b, color.a });	// top right
		// right												 
		vertices.push_back({ position.x - t,			position.y + size.y + t,	color.r, color.g, color.b, color.a });	// top right
		vertices.push_back({ position.x - t,			position.y - t,				color.r, color.g, color.b, color.a });	// bottom right
		vertices.push_back({ position.x + t,			position.y - t,				color.r, color.g, color.b, color.a });	// bottom left
		vertices.push_back({ position.x + t,			position.y - t,				color.r, color.g, color.b, color.a });	// bottom left
		vertices.push_back({ position.x + t,			position.y + size.y + t,	color.r, color.g, color.b, color.a });	// top left 
		vertices.push_back({ position.x - t,			position.y + size.y + t,	color.r, color.g, color.b, color.a });	// top right
		// left												 
		vertices.push_back({ position.x + size.x - t,	position.y + size.y + t,	color.r, color.g, color.b, color.a });	// top right
		vertices.push_back({ position.x + size.x - t,	position.y - t,				color.r, color.g, color.b, color.a });	// bottom right
		vertices.push_back({ position.x + size.x + t,	position.y - t,				color.r, color.g, color.b, color.a });	// bottom left
		vertices.push_back({ position.x + size.x + t,	position.y - t,				color.r, color.g, color.b, color.a });	// bottom left
		vertices.push_back({ position.x + size.x + t,	position.y + size.y + t,	color.r, color.g, color.b, color.a });	// top left 
		vertices.push_back({ position.x + size.x - t,	position.y + size.y + t,	color.r, color.g, color.b, color.a });	// top right

		Vector2 pivot = { rotationOrigin.x, rotationOrigin.y };

		for (int i = 0; i < 24; i++)
		{
			Vertex2D* v = &vertices[i];
			Vector2 pt1 = Vector2::RotateAtPoint({ v->x, v->y }, -rotationAngle, pivot);
			v->x = pt1.x;
			v->y = pt1.y;
		}

		return 24;
	}

	uint32_t PrimitiveUtils::GenerateLine(std::vector<Vertex2D>& vertices, uint32_t offset, glm::vec2 pt1, glm::vec2 pt2, Color color, float thickness, float rotationAngle, glm::vec2 rotationOrigin)
	{
		glm::vec2 dir = glm::normalize(pt2 - pt1);

		glm::vec2 n = {
			dir.x * cos(WYRD_DEG_TO_RAD(90.0)) - dir.y * sin(WYRD_DEG_TO_RAD(90.0)),
			dir.x * sin(WYRD_DEG_TO_RAD(90.0)) + dir.y * cos(WYRD_DEG_TO_RAD(90.0))
		};

		glm::vec2 p1 = pt2 + (n * (thickness * 0.5f));
		glm::vec2 p2 = pt2 - (n * (thickness * 0.5f));
		glm::vec2 p3 = pt1 - (n * (thickness * 0.5f));
		glm::vec2 p4 = pt1 + (n * (thickness * 0.5f));

		vertices.push_back({ p1.x, p1.y, color.r, color.g, color.b, color.a });
		vertices.push_back({ p2.x, p2.y, color.r, color.g, color.b, color.a });
		vertices.push_back({ p3.x, p3.y, color.r, color.g, color.b, color.a });
		vertices.push_back({ p3.x, p3.y, color.r, color.g, color.b, color.a });
		vertices.push_back({ p4.x, p4.y, color.r, color.g, color.b, color.a });
		vertices.push_back({ p1.x, p1.y, color.r, color.g, color.b, color.a });
		
		return 6;
	}

	uint32_t PrimitiveUtils::GenerateVector2D(std::vector<Vertex2D>& vertices, uint32_t offset, glm::vec2 pt1, float angle, float length, Color color, float thickness, float rotationAngle, glm::vec2 rotationOrigin)
	{

		glm::vec2 dir = {
			cos(WYRD_DEG_TO_RAD(-angle)),
			sin(WYRD_DEG_TO_RAD(-angle))
		};

		glm::vec2 pt2 = pt1 + (dir * length);

		glm::vec2 n = {
			dir.x * cos(WYRD_DEG_TO_RAD(90.0)) - dir.y * sin(WYRD_DEG_TO_RAD(90.0)),
			dir.x * sin(WYRD_DEG_TO_RAD(90.0)) + dir.y * cos(WYRD_DEG_TO_RAD(90.0))
		};

		glm::vec2 p1 = pt2 + (n * (thickness * 0.5f));
		glm::vec2 p2 = pt2 - (n * (thickness * 0.5f));
		glm::vec2 p3 = pt1 - (n * (thickness * 0.5f));
		glm::vec2 p4 = pt1 + (n * (thickness * 0.5f));

		vertices.push_back({ p1.x, p1.y, color.r, color.g, color.b, color.a });
		vertices.push_back({ p2.x, p2.y, color.r, color.g, color.b, color.a });
		vertices.push_back({ p3.x, p3.y, color.r, color.g, color.b, color.a });
		vertices.push_back({ p3.x, p3.y, color.r, color.g, color.b, color.a });
		vertices.push_back({ p4.x, p4.y, color.r, color.g, color.b, color.a });
		vertices.push_back({ p1.x, p1.y, color.r, color.g, color.b, color.a });

		return 6;


	}

	uint32_t PrimitiveUtils::GenerateCircle(std::vector<Vertex2D>& vertices, uint32_t offset, glm::vec2 center, float radius, int resolution, Color color, float thickness, float rotationAngle, glm::vec2 rotationOrigin)
	{
		glm::vec2 pt1 = {
				center.x + radius * cos(0.0f),
				center.y + radius * sin(0.0f)
		};

		float increment = 360.0f / resolution;

		for (int i = 1; i < resolution + 1; i++)
		{
			glm::vec2 pt2 = {
				center.x + radius * cos(WYRD_DEG_TO_RAD(increment  * i)),
				center.y + radius * sin(WYRD_DEG_TO_RAD(increment  * i))
			};

			glm::vec2 dir = glm::normalize(pt1 - pt2);

			glm::vec2 n = {
				dir.x * cos(WYRD_DEG_TO_RAD(90.0)) - dir.y * sin(WYRD_DEG_TO_RAD(90.0)),
				dir.x * sin(WYRD_DEG_TO_RAD(90.0)) + dir.y * cos(WYRD_DEG_TO_RAD(90.0))
			};

			glm::vec2 p1 = pt2 + (n * (thickness * 0.5f));
			glm::vec2 p2 = pt2 - (n * (thickness * 0.5f));
			glm::vec2 p3 = pt1 - (n * (thickness * 0.5f));
			glm::vec2 p4 = pt1 + (n * (thickness * 0.5f));

			vertices.push_back({ p1.x, p1.y, color.r, color.g, color.b, color.a });
			vertices.push_back({ p2.x, p2.y, color.r, color.g, color.b, color.a });
			vertices.push_back({ p3.x, p3.y, color.r, color.g, color.b, color.a });
			vertices.push_back({ p3.x, p3.y, color.r, color.g, color.b, color.a });
			vertices.push_back({ p4.x, p4.y, color.r, color.g, color.b, color.a });
			vertices.push_back({ p1.x, p1.y, color.r, color.g, color.b, color.a });

			pt1 = pt2;
		}

		return resolution * 6;
	}
}