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
		TextureRes(std::shared_ptr<Osiris::Texture> texture, std::string name) : _texture(texture), _name(name)
		{

		}

		TextureRes(std::string& filepath) : _filePath(filepath), _loaded(false), _width(0u), _height(0u), _channels(0u), _data(0)
		{
			/* generate the name based on the file name */
			_name = Utils::GetFilename(filepath);

			Load();
		}

		~TextureRes() {}

		// Getters and Setters
		inline std::string& GetName() { return _name; }
		inline void SetName(std::string& name) { _name = name; }
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

		void Load();
		void Reload();
		void Unload();

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