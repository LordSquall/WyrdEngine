#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/renderer/Texture.h>

/* local includes */
#include "core/export.h"
#include "Resource.h"
#include "support/Utils.h"
#include "datamodels/resources/ResourceTypes.h"

namespace Wyrd::Editor
{
	class TextureRes : public Resource
	{
	public:
		TextureRes(const std::string& filepath);

		~TextureRes() = default;

		// Resource overrides
		bool Load() override;
		int GetType() override { return ResourceType::TEXTURE; }

		// Getters and Setters
		inline uint32_t GetWidth() { return _texture->GetWidth(); }
		inline void SetWidth(uint32_t width) { _width = width;  }
		inline uint32_t GetHeight() { return _texture->GetHeight(); }
		inline void SetHeight(uint32_t height) { _height = height; }
		inline uint32_t GetChannels() { return _channels; }
		inline void SetChannels(uint32_t channels) { _channels = channels; }
		inline BYTE* GetData() { return _data; }
		inline void SetData(BYTE* data) { _data = data; }
		inline std::shared_ptr<Texture> GetTexture() { return _texture; }
		inline bool IsLoaded() { return _loaded; }
		inline void SetLoaded(bool loaded) { _loaded = loaded; }

	private:
		bool _loaded;
		std::string _name;
		uint32_t _channels;
		BYTE* _data;

		int32_t _width;
		int32_t _height;

		std::shared_ptr<Texture> _texture;
	};
}