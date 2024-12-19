#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/renderer/Texture.h>

/* local includes */
#include "core/export.h"
#include "Resource.h"
#include "support/Utils.h"
#include "datamodels/resources/ResourceTypes.h"

typedef unsigned char BYTE;

namespace Wyrd::Editor
{
	/**
	 * @brief Texture Resource
	*/
	class TextureRes : public Resource
	{
	public:
		TextureRes(const std::string& name, const UID& uid);
		~TextureRes() = default;

		IOResult Load(const std::string& filepath) override;
		IOResult Load(const jsonxx::Object& obj) override;

		IOResult Save(const std::string& filepath) override;

		ResourceType GetType() override { return ResourceType::TEXTURE; }
		const Icon& GetThumbnail() override;

		/**
		 * @brief Returns the Human readable name for the resource
		 * @return Resource name
		*/
		inline uint32_t GetWidth() { return _texture->GetWidth(); }
		inline uint32_t GetHeight() { return _texture->GetHeight(); }

		inline uint32_t GetChannels() { return _channels; }

		inline BYTE* GetData() { return _data; }
		inline void SetData(BYTE* data) { _data = data; }
		inline std::shared_ptr<Texture> GetTexture() { return _texture; }

	private:
		std::string _name;
		uint32_t _channels;
		BYTE* _data;

		int32_t _width;
		int32_t _height;

		std::shared_ptr<Texture> _texture;
		Icon _thumbnailIcon;
	};
}