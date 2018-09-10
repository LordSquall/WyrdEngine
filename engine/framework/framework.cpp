#include "Framework.h"

/* System Includes */
#include <stdio.h>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <Windows.h>
#include "subsystems\loaders\ConfigLoader.h"
#include "subsystems\window\MSWindow.h"

#include "subsystems\renderer\OGLRenderer.h"
#include "subsystems\renderer\VulkanRenderer.h"

using namespace OrisisEngine;

Framework::Framework()
{
	_logger = nullptr;
	_window = nullptr;
	_renderer = nullptr;
	_currentScene = nullptr;
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
	FrameworkConfig config = FrameworkConfigLoader::LoadFile(configFilePath, _logger);

	if (config.loaded == false)
	{
		_logger->LogError("Framework: ERROR - Unable to load framework configuration");
		return -1;
	}

	/* Initialise a basic window for the application */
	_window = new MSWindow(hInstance);
	_window->RegisterLogger(_logger);

	/* Allocate a renderer based on the supplied renderer configuration */
	_renderer = new OGLRenderer();
	_renderer->RegisterLogger(_logger);

	/* Initialise the renderer subsystem */
	if(_renderer->Initialise(_window) == false)
	{
		_logger->LogError("Framework: ERROR - Unable to initialise renderer");
		return -1;
	}

	/* Create the window. This function will block until window is completed created with all graphical context initialisation complete */
	if (_window->Create(&config) == false)
	{
		_logger->LogError("Framework: ERROR - Unable to create window");
		return -1;
	}

	/* Register the logger for the sprite system */
	_spriteSystem.RegisterLogger(_logger);
	_spriteSystem.SetRenderer(_renderer);

	if (_spriteSystem.LoadResources() == false)
	{
		_logger->LogError("Framework: Unable to load SpriteSystem Shaders");
	}

	return 0;
}

void Framework::Shutdown()
{
	if (_window != NULL)
	{
		_window->Shutdown();
		delete _window;
		_window = NULL;
	}
}

string Framework::GetVersionInfo()
{
	return string("Framework: " + string(ORS_FW_VER_INFO));
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
	glClearColor(0.129f, 0.586f, 0.949f, 1.0f); // Blue
	glClear(GL_COLOR_BUFFER_BIT);

	_spriteSystem.Render();

	_window->PresentFrame();

	return true;
}

void Framework::RegisterLogger(ILogger* logger)
{
	_logger = logger;
	_logger->LogInfo("Logger Registered with Framework");
}

ILogger* Framework::GetLogger()
{
	return _logger;
}

void Framework::AddScene(string name, Scene* scene)
{
	_scenes[name] = scene;
}

void Framework::SetCurrentScene(string name)
{
	if(_currentScene != nullptr)
		_currentScene->OnSceneClosed();

	_currentScene = _scenes[name];

	_currentScene->OnSceneLoaded();
}

SpriteSystem* Framework::GetSpriteSystem()
{
	return &_spriteSystem;
}