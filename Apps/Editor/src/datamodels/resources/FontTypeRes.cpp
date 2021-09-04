#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/renderer/FontType.h>

/* local includes */
#include "FontTypeRes.h"
#include "loaders/TextureLoader.h"
#include "services/ServiceManager.h"
#include "support/BinPacker.h"

/* external includes */
#include <core/Application.h>
#include <ft2build.h>
#include <freetype/freetype.h>

namespace Wyrd::Editor
{
	FontTypeRes::FontTypeRes(const std::filesystem::path& filepath) : Resource(filepath.stem().string(), filepath) {}

	
	bool FontTypeRes::Load()
	{
		/* textures should always be loaded on the main thread */
		if (Application::Get().GetMainThreadID() != std::this_thread::get_id())
		{
			/* swap an event to trigger the load on the main thread */
			ServiceManager::Get<EventService>()->Publish(Events::EventType::LoadAsset, std::make_unique<Events::LoadAssetArgs>(this));
		}
		else
		{
			/* initialise the freetype library */
			FT_Library ft;
			if (FT_Init_FreeType(&ft))
			{
				std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
				return nullptr;
			}

			/* load the ttf face from the file */
			FT_Face face;
			if (FT_New_Face(ft, _path.string().c_str(), 0, &face))
			{
				std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
				return nullptr;
			}

			FT_Set_Pixel_Sizes(face, 0, 48);

			/* create a new font type object */
			_fontType = std::make_shared<FontType>();

			/* initilaise a new  bin packer for the font glyph textures  */
			BinPackerConfig config;
			config.size = { 1024, 1024 };
			config.padding = { 20, 20 };
			config.count = 128;

			BinPacker binPacker(config);

			/* describe and create the font texture */
			TextureDesc fontTextureDesc;
			fontTextureDesc.data = 0;
			fontTextureDesc.width = config.size.x;
			fontTextureDesc.height = config.size.y;
			fontTextureDesc.channels = 1;
			fontTextureDesc.uvWrapping = TextureUVWrap::CLAMP_TO_EDGE;
			fontTextureDesc.maintainCPU = true;
			fontTextureDesc.description = "Font - " + std::filesystem::path(_path).filename().stem().string();

			_fontType->Texture.reset(Texture::Create(fontTextureDesc));

			for (unsigned char c = 0; c < 128; c++)
			{
				/* load character glyph  */
				if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				{
					std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
					continue;
				}

				/* store character data for later use */
				FontType::Character character = {
					glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
					(unsigned int)face->glyph->advance.x,
					{ 0.0f, 0.0f },
					{ 0.0f, 0.0f }
				};

				/* copy the texture data */
				character.data = std::vector(face->glyph->bitmap.buffer, face->glyph->bitmap.buffer + (face->glyph->bitmap.width * face->glyph->bitmap.rows));

				/* add the node area to the bin packer */
				binPacker.AddNode((int)c, character.Size.x, character.Size.y);

				/* store the character info */
				_fontType->GetCharacters().insert(std::pair<char, FontType::Character>(c, character));
			}

			/* pack the actual node set*/
			binPacker.Pack();

			for (unsigned char c = 0; c < 128; c++)
			{
				auto pos = binPacker.GetNodeIdx(c);
				FontType::Character ch = _fontType->GetCharacters()[c];

				/* calculate the uvs */
				glm::vec2 uv1 = { pos.x / config.size.x, pos.y / config.size.y };
				glm::vec2 uv2 = { (pos.x + ch.Size.x) / config.size.x, (pos.y + ch.Size.y) / config.size.y };

				/* store uvs back in character texture */
				_fontType->GetCharacters()[c].uv1 = uv1;
				_fontType->GetCharacters()[c].uv2 = uv2;

				/* if we have data, update the font texture */
				if (!ch.data.empty())
				{
					_fontType->Texture->Update(&ch.data[0], pos.x, pos.y, ch.Size.x, ch.Size.y);
				}
			}

			/* shutdown and release the bin packet */
			binPacker.Shutdown();

			/* shutdown the freetype library */
			FT_Done_Face(face);
			FT_Done_FreeType(ft);

			/* register the font type with the core resource manager */
			Application::Get().GetResources().FontTypes[std::filesystem::path(_path).filename().stem().string()] = _fontType;
		}

		_loaded = true;

		return _loaded;
	}
}
