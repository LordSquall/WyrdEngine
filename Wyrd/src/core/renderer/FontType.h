#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"
#include "core/UID.h"
#include "core/renderer/Texture.h"

/* external includes */
#include <string>
#include <glm/glm.hpp>

namespace Wyrd
{
	class WYRD_LIBRARY_API FontType
	{
	public:
		struct Character {	
			glm::ivec2					Size;       // Size of glyph
			glm::ivec2					Bearing;    // Offset from baseline to left/top of glyph
			unsigned int				Advance;    // Offset to advance to next glyph
			glm::vec2					uv1;
			glm::vec2					uv2;
			std::vector<unsigned char>	data;
		};

	public:
		virtual ~FontType() {}

		inline std::map<char, Character>& GetCharacters() { return _Characters; }
		inline int GetWidth() { return _width; }
		inline int GetHeight() { return _height; }

		std::shared_ptr<Wyrd::Texture>	Texture;
	
	protected:
		std::map<char, Character>	_Characters;
		int _height;
		int _width;
		unsigned char* _data;
		std::string _description;
	};
}