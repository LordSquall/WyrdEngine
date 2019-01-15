#pragma once

#include "osrpch.h"
#include "export.h"
#include "events/Event.h"

#include "graphics/Shader.h"
#include "graphics/Mesh.h"

namespace Osiris
{
	class Renderer
	{
	public:
		virtual ~Renderer() {};

		static Renderer* Create();
		
		virtual unsigned int LoadShader(std::shared_ptr<Shader> shader) = 0;
		virtual unsigned int LoadMesh(std::unique_ptr<Mesh>& mesh) = 0;

		virtual unsigned int RenderMesh(std::unique_ptr<Mesh>& mesh) = 0;
	};
}