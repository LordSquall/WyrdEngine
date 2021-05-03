#pragma once

/* local includes */
#include "core/Log.h"
#include "core/Application.h"

/* Only on windows we want to generate this entry point */
#ifdef WYRD_PLATFORM_WINDOWS

extern Wyrd::Application* Wyrd::CreateApplication();

/**
 * @brief Main Entry point for the application
 * @param argc 
 * @param argv 
*/
void main(int argc, char** argv)
{
	/* initialise logger */
	Wyrd::Log::Init();
	
	/* create the client application */
	auto app = Wyrd::CreateApplication();

	/* enter the main run loop */
	app->Run();
	
	/* delete and clean up application*/
	delete app;
}

#endif