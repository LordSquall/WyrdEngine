#pragma once

#include <string>

namespace Osiris
{
	class Texture
	{
	public:
		virtual ~Texture() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Texture* Create(unsigned char* data, int width, int height, int channels);
	};
}