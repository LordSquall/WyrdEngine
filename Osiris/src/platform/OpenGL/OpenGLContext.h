#pragma once

#include "core/renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Osiris
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		void Init() override;

		void SwapBuffer() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}