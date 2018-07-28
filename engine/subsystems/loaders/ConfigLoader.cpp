#include "ConfigLoader.h"

#include <fstream>
#include <string>

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

FrameworkConfig FrameworkConfigLoader::LoadFile(string filePath, ILogger* logger)
{
	FrameworkConfig config(800, 600, 0, 0, string("Application"));		/* Returnable configuration  */
	string line;														/* Current processing line   */
				
	ifstream fileStream(filePath);										/* Configuration file stream */

	while (getline(fileStream, line))
	{
		size_t dPos = line.find_first_of('=');

		if (line.substr(0, dPos).compare("width") == 0) {
			config.width = stoi(line.substr(dPos + 1));
		} else if (line.substr(0, dPos).compare("height") == 0) {
			config.height = stoi(line.substr(dPos + 1));
		} else if (line.substr(0, dPos).compare("x") == 0) {
			config.x = stoi(line.substr(dPos + 1));
		} else if (line.substr(0, dPos).compare("y") == 0) {
			config.y = stoi(line.substr(dPos + 1));
		} else if (line.substr(0, dPos).compare("title") == 0) {
			strcpy(config.title, line.substr(dPos + 1).c_str());
		}
	}

	fileStream.close();
	
	return FrameworkConfig(config);
}