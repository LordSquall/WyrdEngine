
/* Core orisis includes */
#include <core/Log.h>

/* local project includes */
#include "import/Importer.h"

void Importer::Read(std::ifstream& stream, CoreFormat_Config& config)
{
	config.version = StringHelper(stream);
}
