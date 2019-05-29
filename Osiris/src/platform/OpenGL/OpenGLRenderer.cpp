#pragma once

#include "osrpch.h"
#include "OpenGLRenderer.h"

#include <glad/glad.h>

namespace Osiris {

	OpenGLRenderer::OpenGLRenderer()
	{
		OSR_CORE_TRACE("Renderer Created");
	}

	Renderer* Renderer::Create()
	{
		return new OpenGLRenderer();
	}

	OpenGLRenderer::~OpenGLRenderer()
	{

	}
}
