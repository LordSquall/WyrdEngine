#include "wyrdpch.h"

#include "TextureLoader.h"

#ifdef WYRD_PLATFORM_WINDOWS
#include <SOIL.h>
#elif WYRD_PLATFORM_LINUX
#include <SOIL/SOIL.h>
#endif

namespace Wyrd::Editor
{
	TextureLoader::Result TextureLoader::Load(const std::filesystem::path& filepath, int32_t& width, int32_t& height, uint32_t& channels, unsigned char** data)
	{
		int w, h, c;
		unsigned char* d;

		/* check the file exsits */
		if (!Utils::FileExists(filepath))
		{
			return Result::FileNotFound;
		}

		/* perform the actual load operation */
		d = SOIL_load_image(filepath.string().c_str(), &w, &h, &c, 4);

		if (d == nullptr)
		{
			printf("SOIL loading error: '%s'\n", SOIL_last_result());
			return Result::FileMalformed;
		}

		/* set texture details */
		width = w;
		height = h;
		channels = c;
		(*data) = d;
		
		return Result::Success;
	}
}