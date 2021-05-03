#pragma once
#include "wyrdpch.h"
#include "core/Log.h"

#include <glad/glad.h>

namespace Wyrd
{
	class OpenGLError
	{
	public:
		inline static void Resolve(GLenum value)
		{
			switch (value)
			{
			case GL_INVALID_ENUM:
				WYRD_CORE_ERROR("OpenGL: Invalid Enum");
				break;
			case GL_INVALID_VALUE:
				WYRD_CORE_ERROR("OpenGL: Invalid Value");
				break;
			case GL_INVALID_OPERATION:
				WYRD_CORE_ERROR("OpenGL: Invalid Operation");
				break;
			case GL_STACK_OVERFLOW:
				WYRD_CORE_ERROR("OpenGL: Stack Overflow");
				break;
			case GL_STACK_UNDERFLOW:
				WYRD_CORE_ERROR("OpenGL: Stack Underflow");
				break;
			case GL_OUT_OF_MEMORY:
				WYRD_CORE_ERROR("OpenGL: Out of Memory");
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				WYRD_CORE_ERROR("OpenGL: Invalid Framebuffer Operation");
				break;
			case GL_CONTEXT_LOST:
				WYRD_CORE_ERROR("OpenGL: Context Lost");
				break;
			default:
				WYRD_CORE_ERROR("Unknown OpenGL Error Value:{0}", value);
				break;
			}
		}
	};
}