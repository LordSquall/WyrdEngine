#pragma once

#include "core/export.h"

#include "Vertex.h"

namespace Osiris {

	class OSIRIS_API Sprite
	{
	public:
		Sprite();
		Sprite(float width, float height) : width(width), height(height) { }

		virtual ~Sprite();

		std::vector<Vertex> vertices;

		uint32_t vbo;

		float width;
		float height;
	};
}