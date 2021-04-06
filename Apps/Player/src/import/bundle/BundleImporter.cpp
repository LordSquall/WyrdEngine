#pragma once

/* Core orisis includes */
#include <core/Log.h>

/* local project includes */
#include "import/Importer.h"

void Importer::Read(std::ifstream& stream, BundleFormat_Config& config)
{
	config.version = StringHelper(stream);
}
