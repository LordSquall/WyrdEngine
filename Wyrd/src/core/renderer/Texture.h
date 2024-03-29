#pragma once

/* local includes */
#include "core/export.h"
#include "core/UID.h"
#include "core/renderer/ResourceBase.h"

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
		std::string name;
		int width;
		int height;
		int channels;
		bool maintainCPU;
		TextureUVWrap uvWrapping;
		Wyrd::UID guid;
		std::string description;

		TextureDesc() : data(nullptr), name(""), width(0), height(0), channels(0), maintainCPU(false), uvWrapping(TextureUVWrap::REPEAT), guid(""), description("") { }
	};

	class WYRD_LIBRARY_API Texture : public ResourceBase
	{
	public:
		virtual ~Texture() {}

		virtual void Bind(unsigned int idx = 0) const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetHandle() const = 0;

		virtual void Update(unsigned char* data, int xOffset, int yOffset, int width, int height) = 0;

		virtual void Delete() = 0;

		inline int GetWidth() const { return _width; }
		inline void SetWidth(int width) { _width = width; }
		inline int GetHeight() const { return _height; }
		inline void SetHeight(int height) { _height = height; }
		inline int GetChannels() const { return _channels; }

		inline unsigned char* GetData() const { return _data; }

		inline const std::string& GetDescription() const { return _description; }

		static Texture* Create(const TextureDesc& desc);

	protected:
		int _height;
		int _width;
		int _channels;
		unsigned char* _data;
		std::string _description;
		bool _maintainCPU;
	};
}