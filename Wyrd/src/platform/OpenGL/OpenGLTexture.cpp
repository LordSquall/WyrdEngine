#include "wyrdpch.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>

namespace Wyrd
{

	OpenGLTexture::OpenGLTexture(const TextureDesc& desc)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

		/* Store width, height and data */
		_name = desc.name;
		_width = desc.width;
		_height = desc.height;
		_channels = desc.channels;
		_uid = desc.guid;
		_description = desc.description;
		_maintainCPU = desc.maintainCPU;
		
		/* we have requested to maintain a CPU copy of the texture so we need to create the storage */
		if (_maintainCPU)
		{
			size_t bufferSize = sizeof(unsigned char) * (_width * _height * _channels);
			_data = (unsigned char*)malloc(bufferSize);
			if (desc.data == 0)
			{
				memset(_data, 0, bufferSize);
			}
			else
			{
				memcpy(_data, desc.data, bufferSize);
			}
		}
		else
		{
			_data = desc.data;
		}
		

		glGenTextures(1, &_rendererID);
		glBindTexture(GL_TEXTURE_2D, _rendererID);
		glObjectLabel(GL_TEXTURE, _rendererID, -1, _description.c_str());

		switch (desc.uvWrapping)
		{
		case CLAMP_TO_EDGE: 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
			break;
		case CLAMP_TO_BORDER: 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); 
			break;
		case MIRRORED_REPEAT: 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			break;
		case MIRRORED_CLAMP: 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRROR_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRROR_CLAMP_TO_EDGE);
			break;
		case REPEAT: 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
			break;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		switch (_channels)
		{
		case 1:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, _width, _height, 0, GL_RED, GL_UNSIGNED_BYTE, _data);
			break;
		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);
			break;
		case 4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);
			break;
		}
	}

	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &_rendererID);

		if (_maintainCPU)
		{
			free(_data);
			_data = nullptr;
		}
	}

	void OpenGLTexture::Bind(unsigned int idx) const
	{
		glActiveTexture(GL_TEXTURE0 + idx);
		glBindTexture(GL_TEXTURE_2D, _rendererID);
	}

	void OpenGLTexture::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	uint32_t OpenGLTexture::GetHandle() const
	{
		return _rendererID;
	}

	void OpenGLTexture::Update(unsigned char* data, int xOffset, int yOffset, int width, int height)
	{
		glBindTexture(GL_TEXTURE_2D, _rendererID);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
		glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, width, height, GL_RED, GL_UNSIGNED_BYTE, data);

		if (_maintainCPU)
		{
			int initialOffset = xOffset + (yOffset * _width * _channels);

			for (int i = 0; i < height; i++)
			{
				memcpy(&_data[initialOffset + (i * (_width * _channels))], &data[i * (width * _channels)], sizeof(unsigned char) * (width * _channels));
			}
		}
	}


	void OpenGLTexture::Delete()
	{
		glDeleteTextures(1, &_rendererID);
	}
}
