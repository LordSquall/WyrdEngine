#pragma once

/* local includes */
#include "core/export.h"

/* external includes */
#include <string>

namespace Wyrd
{
	class WYRD_LIBRARY_API VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetAttribute(uint32_t id, uint32_t offset, uint32_t size, uint32_t stride) const = 0;

		static VertexArray* Create();
	};
}