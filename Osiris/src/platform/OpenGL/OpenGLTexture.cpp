#include "osrpch.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>

namespace Osiris
{

	OpenGLTexture::OpenGLTexture(uint32_t uid, unsigned char* data, int width, int height, int channels, const std::string& description)
	{
		_uid = uid;

		glGenTextures(1, &_rendererID);
		glBindTexture(GL_TEXTURE_2D, _rendererID);
		//glObjectLabel(GL_TEXTURE, _rendererID, -1, description.c_str());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		switch (channels)
		{
		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;
		case 4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;
		}
	}

	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &_rendererID);
	}

	void OpenGLTexture::Bind() const
	{
		glActiveTexture(GL_TEXTURE0);
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
		glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}


	void OpenGLTexture::Delete()
	{
		glDeleteTextures(1, &_rendererID);
	}
}
