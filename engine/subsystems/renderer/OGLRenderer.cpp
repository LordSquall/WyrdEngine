#include "OGLRenderer.h"


using namespace OrisisEngine;

OGLRenderer::OGLRenderer()
{
}

OGLRenderer::OGLRenderer(const OGLRenderer &obj)
{
}

OGLRenderer::~OGLRenderer()
{
}

void OGLRenderer::RegisterLogger(ILogger* logger)
{
	_logger = logger;
}

bool OGLRenderer::Initialise(IWindow* window)
{
	_logger->LogInfo("OGLRenderer: Initialise Called");
	return false;
}

void OGLRenderer::Shutdown()
{
}