#pragma once

/* local includes */
#include "core/export.h"
#include "core/UID.h"

/* external includes */
#include <string>

namespace Wyrd
{
	enum TextureUVWrap
	{
		CLAMP_TO_EDGE,
		CLAMP_TO_BORDER,
		MIRRORED_REPEAT,
		MIRRORED_CLAMP,
		REPEAT
	};

	struct TextureDesc
	{
		unsigned char* data;
		int width;
		int height;
		int channels;
		TextureUVWrap uvWrapping;
		std::string description;

		TextureDesc() : data(nullptr), width(0), height(0), channels(0), uvWrapping(TextureUVWrap::REPEAT), description("") { }
	};

	class WYRD_LIBRARY_API Texture
	{
	public:
		virtual ~Texture() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetHandle() const = 0;

		virtual void Update(unsigned char* data, int xOffset, int yOffset, int width, int height) = 0;

		virtual void Delete() = 0;

		inline int GetWidth() const { return _width; }
		inline void SetWidth(int width) { _width = width; }
		inline int GetHeight() const { return _height; }
		inline void SetHeight(int height) { _height = height; }

		inline UID GetUID() { return _uid; }
		inline void SetUID(UID uid) { _uid = uid; }

		inline unsigned char* GetData() const { return _data; }

		inline const std::string& GetDescription() const { return _description; }

		static Texture* Create(const TextureDesc& desc);

	protected:
		UID _uid;
		int _height;
		int _width;
		unsigned char* _data;
		std::string _description;
	};
}