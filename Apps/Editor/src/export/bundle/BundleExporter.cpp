#pragma once

/* Core orisis includes */
#include <core/Log.h>

/* local project includes */
#include "export/Exporter.h"

void Exporter::Write(std::ofstream& stream, BundleFormat_Config& config)
{
	StringHelper(stream, config.version);
}
