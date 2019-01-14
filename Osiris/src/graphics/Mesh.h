#pragma once

#include "core/export.h"

#include "Vertex.h"

namespace Osiris {

	class OSIRIS_API Mesh
	{
	public:
		Mesh();

		virtual ~Mesh();


		static std::unique_ptr<Mesh> CreateFromFile(const std::string& filePath);

		std::vector<Vertex> vertices;

		uint32_t vbo;
	};
}