#pragma once

/* local includes */
#include "core/export.h"
#include "core/UID.h"

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

		inline const UID GetUID() { return _uid; }
		inline void SetUID(UID uid) { _uid = uid; }

		static std::shared_ptr<Texture> Create(unsigned char* data, int width, int height, int channels, const std::string& description);

	protected:
		UID _uid;
	};
}