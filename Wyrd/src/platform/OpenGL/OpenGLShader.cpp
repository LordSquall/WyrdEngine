/* local includes */
#include "wyrdpch.h"
#include "OpenGLShader.h"
#include "OpenGLError.h"

/* external includes */
#include <glad/glad.h>

namespace Wyrd
{
	OpenGLShader::OpenGLShader(ShaderDesc desc)
		: m_RendererHandle(0)
	{
		_uid = desc.resource.guid;
		_name = desc.resource.name;

		// Store the source code
		_Source[ShaderStage::Vertex] = desc.vertexSrc;
		_Source[ShaderStage::Fragment] = desc.fragmentSrc;
	}

	bool OpenGLShader::Build(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		// Store the source code
		_Source[ShaderStage::Vertex] = vertexSrc;
		_Source[ShaderStage::Fragment] = fragmentSrc;

		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar *source = (const GLchar *)vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.
			std::string msg(infoLog.begin(), infoLog.end());
			WYRD_CORE_ERROR(msg);

			// In this simple program, we'll just leave
			return false;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar *)fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.
			std::string msg(infoLog.begin(), infoLog.end());
			WYRD_CORE_ERROR(msg);

			// In this simple program, we'll just leave
			return false;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_RendererHandle = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(m_RendererHandle, vertexShader);
		glAttachShader(m_RendererHandle, fragmentShader);

		// Link our program
		glLinkProgram(m_RendererHandle);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_RendererHandle, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RendererHandle, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererHandle, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_RendererHandle);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.
			std::string msg(infoLog.begin(), infoLog.end());
			WYRD_CORE_ERROR(msg);

			// In this simple program, we'll just leave
			return false;
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_RendererHandle, vertexShader);
		glDetachShader(m_RendererHandle, fragmentShader);
		return true;
	}

	void OpenGLShader::Bind()
	{
		glUseProgram(m_RendererHandle);

		glUniform1i(glGetUniformLocation(m_RendererHandle, "s_Texture"), 0); // set it manually

		glUniform1i(glGetUniformLocation(m_RendererHandle, "u_Texture1"), 0); // set it manually
		glUniform1i(glGetUniformLocation(m_RendererHandle, "u_Texture2"), 1); // set it manually
	}

	void OpenGLShader::Unbind()
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetVPMatrix(const glm::mat4& mat)
	{
		GLuint location = glGetUniformLocation(m_RendererHandle, "viewProjection");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));

		GLenum err = glGetError();

		if (err != GL_NO_ERROR)
			OpenGLError::Resolve(err);
	}

	void OpenGLShader::SetModelMatrix(const glm::mat4& mat)
	{
		GLuint location = glGetUniformLocation(m_RendererHandle, "model");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));

		GLenum err = glGetError();

		if (err != GL_NO_ERROR)
			OpenGLError::Resolve(err);
	}

	void OpenGLShader::SetUniformFloat(const std::string& name, float val)
	{
		GLuint location = glGetUniformLocation(m_RendererHandle, name.c_str());
		glUniform1f(location, val);

		GLenum err = glGetError();

		if (err != GL_NO_ERROR)
			OpenGLError::Resolve(err);
	}

	void OpenGLShader::SetUniformVec2(const std::string& name, glm::vec2& vec2)
	{
		GLuint location = glGetUniformLocation(m_RendererHandle, name.c_str());
		glUniform2fv(location, 1, glm::value_ptr(vec2));

		GLenum err = glGetError();

		if (err != GL_NO_ERROR)
			OpenGLError::Resolve(err);
	}

	void OpenGLShader::SetUniformVec3(const std::string& name, glm::vec3& vec3)
	{
		GLuint location = glGetUniformLocation(m_RendererHandle, name.c_str());
		glUniform3fv(location, 1, glm::value_ptr(vec3));

		GLenum err = glGetError();

		if (err != GL_NO_ERROR)
			OpenGLError::Resolve(err);
	}

	void OpenGLShader::SetUniformVec4(const std::string& name, glm::vec4& vec4)
	{
		GLuint location = glGetUniformLocation(m_RendererHandle, name.c_str());
		glUniform4fv(location, 1, glm::value_ptr(vec4));

		GLenum err = glGetError();

		if (err != GL_NO_ERROR)
			OpenGLError::Resolve(err);
	}


	void OpenGLShader::SetUniformColor(const std::string& name, const Color& color)
	{
		GLuint location = glGetUniformLocation(m_RendererHandle, name.c_str());
		glUniform4fv(location, 1, (GLfloat*)&color);

		GLenum err = glGetError();

		if (err != GL_NO_ERROR)
			OpenGLError::Resolve(err);
	}

	void OpenGLShader::SetMatrix(const std::string& name, const glm::mat4& mat)
	{
		GLuint location = glGetUniformLocation(m_RendererHandle, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));

		GLenum err = glGetError();

		if (err != GL_NO_ERROR)
			OpenGLError::Resolve(err);
	}
}