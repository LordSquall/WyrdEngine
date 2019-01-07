#pragma once

#ifdef OSR_PLATFORM_WINDOWS

extern Osiris::Application* Osiris::CreateApplication();

void main(int argc, char** argv)
{
	Osiris::Log::Init();
	
	auto app = Osiris::CreateApplication();
	app->Run();
	delete app;
}

#endif