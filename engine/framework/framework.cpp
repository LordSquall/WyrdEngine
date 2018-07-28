#include "Framework.h"

/* System Includes */
#include <stdio.h>

#include <Windows.h>
#include "subsystems\loaders\ConfigLoader.h"
#include "subsystems\window\MSWindow.h"
#include "subsystems\renderer\DX11Renderer.h"
#include "subsystems\renderer\OGLRenderer.h"

using namespace OrisisEngine;

Framework::Framework()
{
	_logger = NULL;
	_window = NULL;
}

Framework::Framework(const Framework &obj)
{
}

Framework::~Framework()
{
	_logger->LogInfo("Framework: Deconstructor Called");
}

int Framework::Initialise(HINSTANCE hInstance, string configFilePath)
{
	_logger->LogInfo("Framework: Initialise Called");

	/* Load the configuration file */
	FrameworkConfig config = FrameworkConfigLoader::LoadFile(configFilePath);

	/* Initialise a basic window for the application */
	_window = new MSWindow(hInstance);
	_window->RegisterLogger(_logger);

	/* Allocate a renderer based on the supplied renderer configuration */
#if defined _ORS_RENDERER_OPENGL_
	_renderer = new OGLRenderer();
	_renderer->RegisterLogger(_logger);
#elif defined _ORS_RENDERER_DX11_
	renderer = new DX11Renderer();
#elif defined _ORS_RENDERER_VULKAN_
	renderer = new VulkanRenderer();
#else
	_logger->LogError("Framework: No Renderer symbol found.");
#endif

	/*Initialise the renderer subsystem */
	_renderer->Initialise();


	_window->Create(&config);

	return 0;
}

bool Framework::Run()
{
	/* Check to see if window has been marked as closed */
	while(_window->IsRunning())
	{
		ExecuteFrame();
	}

	return true;
}

bool Framework::ExecuteFrame()
{
	_logger->LogInfo("FRAME");
	return true;
}

int Framework::Shutdown()
{
	if (_window != NULL)
	{
		_window->Shutdown();
		delete _window;
		_window = NULL;
	}
	return 0;
}

void Framework::RegisterLogger(ILogger* logger)
{
	_logger = logger;
	_logger->LogInfo("Logger Registered with Framework");
}