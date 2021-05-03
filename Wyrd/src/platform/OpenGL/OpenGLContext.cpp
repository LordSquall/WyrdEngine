#pragma once

#include "wyrdpch.h"
#include "core/Log.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Wyrd {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{

	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		OSR_CORE_ASSERT(status, "Failed to initialise GLAD!");

		WYRD_CORE_INFO("Graphics Context Info: OpenGL");
		WYRD_CORE_INFO("   Vendor: {0}", glGetString(GL_VENDOR));
		WYRD_CORE_INFO("   Renderer: {0}", glGetString(GL_RENDERER));
		WYRD_CORE_INFO("   Version: {0}", glGetString(GL_VERSION));

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLContext::SwapBuffer()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}
