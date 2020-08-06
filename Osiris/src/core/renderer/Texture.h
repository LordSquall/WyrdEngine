#pragma once

/* local includes */
#include "core/export.h"

/* external includes */
#include <string>

namespace Osiris
{
	class OSR_LIBRARY_API Texture
	{
	public:
		virtual ~Texture() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetHandle() const = 0;

		virtual void Update(unsigned char* data, int xOffset, int yOffset, int width, int height) = 0;

		virtual void Delete() = 0;

		inline uint32_t GetUID() { return _uid; }

		static std::shared_ptr<Texture> Create(unsigned char* data, int width, int height, int channels, const std::string& description);

	protected:
		uint32_t _uid = 0u;

	private:
		static uint32_t _nextUid;
	};
}