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
		FontTypeRes(const std::filesystem::path& filepath, const UID& uid);

		~FontTypeRes() = default;

		// Resource overrides
		int Load() override;
		ResourceType GetType() override { return Wyrd::Editor::ResourceType::FONT; }

	private:
		std::string _name;
		uint32_t _channels;
		unsigned char* _data;

		int32_t _width;
		int32_t _height;

		std::shared_ptr<FontType> _fontType;
	};
}