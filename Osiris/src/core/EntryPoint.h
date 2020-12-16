#pragma once

/* local includes */
#include "core/Log.h"
#include "core/Application.h"

/* Only on windows we want to generate this entry point */
#ifdef OSR_PLATFORM_WINDOWS

extern Osiris::Application* Osiris::CreateApplication();

/**
 * @brief Main Entry point for the application
 * @param argc 
 * @param argv 
*/
void main(int argc, char** argv)
{
	/* initialise logger */
	Osiris::Log::Init();
	
	/* create the client application */
	auto app = Osiris::CreateApplication();

	/* enter the main run loop */
	app->Run();
	
	/* delete and clean up application*/
	delete app;
}

#endif