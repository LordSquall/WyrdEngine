#pragma once

#include "wyrdpch.h"

#include "core/renderer/Renderer.h"
#include "core/renderer/FontType.h"

#include <ft2build.h>
#include <freetype/freetype.h>

namespace Wyrd
{
	std::shared_ptr<FontType> FontType::CreateFromTTFFile(const std::string& filename)
	{
		std::shared_ptr<FontType> font = std::make_shared<FontType>();

		FT_Library ft;
		if (FT_Init_FreeType(&ft))
		{
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
			return nullptr;
		}

		auto cwd = std::filesystem::current_path();

		FT_Face face;
		if (FT_New_Face(ft, filename.c_str(), 0, &face))
		{
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
			return nullptr;
		}

		FT_Set_Pixel_Sizes(face, 0, 48);

		for (unsigned char c = 0; c < 128; c++)
		{
			// load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}

			// now store character for later use
			Character character = {
				nullptr,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				face->glyph->advance.x
			};


			TextureDesc textureDesc;
			textureDesc.data = face->glyph->bitmap.buffer;
			textureDesc.width = face->glyph->bitmap.width;
			textureDesc.height = face->glyph->bitmap.rows;
			textureDesc.channels = 1;
			textureDesc.uvWrapping = TextureUVWrap::CLAMP_TO_EDGE;
			textureDesc.description = "Glyph - " + c;

			character.Texture.reset(Texture::Create(textureDesc));

			font->GetCharacters().insert(std::pair<char, Character>(c, character));
		}

		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		return font;
	}
}