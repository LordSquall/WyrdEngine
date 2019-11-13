#pragma once
#include "osrpch.h"

#include <glad/glad.h>

namespace Osiris
{
	class OpenGLError
	{
	public:
		inline static void Resolve(GLenum value)
		{
			switch (value)
			{
			case GL_INVALID_ENUM:
				OSR_CORE_ERROR("OpenGL: Invalid Enum");
				break;
			case GL_INVALID_VALUE:
				OSR_CORE_ERROR("OpenGL: Invalid Value");
				break;
			case GL_INVALID_OPERATION:
				OSR_CORE_ERROR("OpenGL: Invalid Operation");
				break;
			case GL_STACK_OVERFLOW:
				OSR_CORE_ERROR("OpenGL: Stack Overflow");
				break;
			case GL_STACK_UNDERFLOW:
				OSR_CORE_ERROR("OpenGL: Stack Underflow");
				break;
			case GL_OUT_OF_MEMORY:
				OSR_CORE_ERROR("OpenGL: Out of Memory");
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				OSR_CORE_ERROR("OpenGL: Invalid Framebuffer Operation");
				break;
			case GL_CONTEXT_LOST:
				OSR_CORE_ERROR("OpenGL: Context Lost");
				break;
			default:
				OSR_CORE_ERROR("Unknown OpenGL Error Value:{0}", value);
				break;
			}
		}
	};
}