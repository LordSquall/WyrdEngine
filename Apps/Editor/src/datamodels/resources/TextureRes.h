#pragma once

#include "osrpch.h"

#include "core/export.h"
#include "Resource.h"

#include "support/Utils.h"
#include "core/renderer/Texture.h"

namespace Osiris::Editor
{
	class TextureRes : public Resource
	{
	public:
		TextureRes(std::shared_ptr<Osiris::Texture> texture, std::string name);

		TextureRes(const std::string& filepath, bool delayLoad = false);

		~TextureRes() = default;

		// Resource overrides
		bool Load() override;

		// Getters and Setters
		inline std::string& GetFilePath() { return _filePath; }
		inline void SetFilePath(std::string& path) { _filePath = path; }
		inline uint32_t GetWidth() { return _width; }
		inline void SetWidth(uint32_t width) { _width = width; }
		inline uint32_t GetHeight() { return _height; }
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
		std::string _filePath;
		uint32_t _width;
		uint32_t _height;
		uint32_t _channels;
		BYTE* _data;

		std::shared_ptr<Texture> _texture;
	};
}