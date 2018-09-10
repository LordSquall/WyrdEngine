/* System Includes */
#include <windows.h>
#include <windowsx.h>

/* Orisis Engine Framework Includes */
#include "framework\framework.h"

/* Local Implemented Orisis Classes */
#include "SampleScene.h"
#include "ConsoleLogger.h"

using namespace OrisisEngine;

/* Program Entry Point */
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	/* Create a Console Logger */
	ILogger* logger = new ConsoleLogger();

	/* Initialise a Framework object */
	Framework* framework = new Framework();

	/* Register the logger to ensure we get all the messages as early as possible */
	framework->RegisterLogger(logger);

	/* The hardcoded test scene */
	SampleScene scene(framework);

	/* Retrieve framework version info */
	logger->LogInfo(framework->GetVersionInfo());

	/* Attempt the initialise the framework */
	if (framework->Initialise(hInstance, "../application/config.ini") != 0)
	{
		/* We have failed to initialise */
		logger->LogError("Framework Initialise Failed");
		
		/* Delete the framework and logger objects */
		delete framework;
		delete logger;

		exit(0);
	}

	/* Add the scene to the framework */
	framework->AddScene("SampleScene", &scene);

	/*  Set the default scene */
	framework->SetCurrentScene("SampleScene");

	/* Run the framework - This is a blocking call */
	framework->Run();

	/* Shutdown the framwork */
	framework->Shutdown();

	/* Delete framework and logger objects */
	delete framework;
	delete logger;

	return 0;
}