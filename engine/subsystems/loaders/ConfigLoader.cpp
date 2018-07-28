#include "ConfigLoader.h"

using namespace OrisisEngine;

FrameworkConfigLoader::FrameworkConfigLoader()
{
}

FrameworkConfigLoader::FrameworkConfigLoader(const FrameworkConfigLoader &obj)
{
}

FrameworkConfigLoader::~FrameworkConfigLoader()
{
}

FrameworkConfig FrameworkConfigLoader::LoadFile(string filePath)
{
	// TODO file loaded not yet implemented, returning hardcarded configuration
	return FrameworkConfig(800, 600, 0, 0);
}