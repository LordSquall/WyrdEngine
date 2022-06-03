#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/renderer/FontType.h>

/* local includes */
#include "core/export.h"
#include "Resource.h"
#include "support/Utils.h"
#include "datamodels/resources/ResourceTypes.h"

namespace Wyrd::Editor
{
	class FontTypeRes : public Resource
	{
	public:
		FontTypeRes(const std::filesystem::path& filepath);

		~FontTypeRes() = default;

		// Resource overrides
		bool Load() override;
		int GetType() override { return ResourceType::FONT; }

	private:
		bool _loaded;
		std::string _name;
		uint32_t _channels;
		unsigned char* _data;

		int32_t _width;
		int32_t _height;

		std::shared_ptr<FontType> _fontType;
	};
}