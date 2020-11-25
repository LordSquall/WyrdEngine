#include "osrpch.h"
#include "OpenGLRenderer.h"
#include "core/Log.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

namespace Osiris
{
	OpenGLRenderer::OpenGLRenderer()
	{
		std::ostringstream stringStream;
		stringStream << glGetString(GL_VENDOR);
		_vendorInfo.vendor = stringStream.str();
		stringStream.str(std::string());

		stringStream << glGetString(GL_VERSION);
		_vendorInfo.version = stringStream.str();
		stringStream.str(std::string());

		stringStream << glGetString(GL_RENDERER);
		_vendorInfo.renderer = stringStream.str();
		stringStream.str(std::string());

		/* Initialise the debug renderer objects */
		std::ifstream vertexStream("../../Osiris/res/shaders/debug2D.vs");
		std::string vertexSrc((std::istreambuf_iterator<char>(vertexStream)), std::istreambuf_iterator<char>());
		const GLchar* vertexSrcPtr = vertexSrc.c_str();

		std::ifstream fragmentStream("../../Osiris/res/shaders/debug2D.fs");
		std::string fragmentSrc((std::istreambuf_iterator<char>(fragmentStream)), std::istreambuf_iterator<char>());
		const GLchar* fragmentSrcPtr = fragmentSrc.c_str();

		int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSrcPtr, NULL);
		glCompileShader(vertexShader);
		// check for shader compile errors
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			OSR_ERROR("Debug Vertex Shader Compilation Failed: {0}", infoLog);
		}
		// fragment shader
		int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSrcPtr, NULL);
		glCompileShader(fragmentShader);
		// check for shader compile errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			OSR_ERROR("Debug Fragment Shader Compilation Failed: {0}", infoLog);
		}
		// link shaders
		_debugShader = glCreateProgram();
		glAttachShader(_debugShader, vertexShader);
		glAttachShader(_debugShader, fragmentShader);
		glLinkProgram(_debugShader);
		// check for linking errors
		glGetProgramiv(_debugShader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(_debugShader, 512, NULL, infoLog);
			OSR_ERROR("Debug Shader Linking Failed: {0}", infoLog);
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		OSR_INFO("Debug Shader Compiled Succesfully");

		_debugVPMatLoc = glGetUniformLocation(_debugShader, "viewProjection");
		_debugModelMatLoc = glGetUniformLocation(_debugShader, "model");
		_debugColorLoc = glGetUniformLocation(_debugShader, "color");

		glGenVertexArrays(1, &_debugVAO);
		glGenBuffers(1, &_debugVBO);
		glGenBuffers(1, &_debugEBO);

		glBindVertexArray(_debugVAO);

		float vertices[] = {
			 0.5f,  0.5f, 0.0f,  // top right
			 0.5f, -0.5f, 0.0f,  // bottom right
			-0.5f, -0.5f, 0.0f,  // bottom left
			-0.5f,  0.5f, 0.0f   // top left 
		};

		unsigned int indices[] = {  // note that we start from 0!
			0, 1, 3,  // first Triangle
			1, 2, 3   // second Triangle
		};

		glBindBuffer(GL_ARRAY_BUFFER, _debugVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _debugEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	OpenGLRenderer::~OpenGLRenderer()
	{
		if (_debugShader != 0)
		{
			glDeleteShader(_debugShader);
			_debugShader = 0;
		}
	}

	void OpenGLRenderer::Clear(float r, float g, float b) const
	{
		glClearColor(r, g, b, 1);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLRenderer::DrawElements(RendererDrawType type, uint32_t count) const
	{
		GLuint primitiveType = 0;

		/* map renderer primitive to opengl primitive */
		switch (type)
		{
		case RendererDrawType::Triangles:
			primitiveType = GL_TRIANGLES;
			break;
		case RendererDrawType::TriangleStrip:
			primitiveType = GL_TRIANGLE_STRIP;
			break;
		}

		glDrawElements(primitiveType, count, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRenderer::DrawRect(const Rect& rect, const glm::vec4& color, const glm::mat4& vpMatrix) const
	{
		float vertices[] = {
			rect.position.x,				rect.position.y + rect.size.y,	0.0f,	// top right
			rect.position.x,				rect.position.y,				0.0f,	// bottom right
			rect.position.x + rect.size.x,	rect.position.y,				0.0f,	// bottom left
			rect.position.x + rect.size.x,	rect.position.y + rect.size.y,	0.0f	// top left 
		};

		glBindBuffer(GL_ARRAY_BUFFER, _debugVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glUseProgram(_debugShader);
		glUniformMatrix4fv(_debugVPMatLoc, 1, GL_FALSE, glm::value_ptr(vpMatrix));
		glUniformMatrix4fv(_debugModelMatLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
		glUniform4fv(_debugColorLoc, 1, glm::value_ptr(color));

		glBindVertexArray(_debugVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void OpenGLRenderer::DrawRay(const Ray& ray, const glm::vec4& color, float length, float thickness, const glm::mat4& vpMatrix) const
	{
		float vertices[] = {
			(thickness / 2.0f),		length,	0.0f,	// top right
			(thickness / 2.0f),		0.0,	0.0f,	// bottom right
			-(thickness / 2.0f),	0.0,	0.0f,	// bottom left
			-(thickness / 2.0f),	length,	0.0f   // top left 
		};

		glm::mat4 tranlate = glm::translate(glm::mat4(1.0f), glm::vec3(ray.origin.x, ray.origin.y, 0)); // where x, y, z is axis of rotation (e.g. 0 1 0)
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), atan2(-ray.dir.x, ray.dir.y), glm::vec3(0, 0, 1)); // where x, y, z is axis of rotation (e.g. 0 1 0)
		glBindBuffer(GL_ARRAY_BUFFER, _debugVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glUseProgram(_debugShader);
		glUniformMatrix4fv(_debugVPMatLoc, 1, GL_FALSE, glm::value_ptr(vpMatrix));
		glUniformMatrix4fv(_debugModelMatLoc, 1, GL_FALSE, glm::value_ptr(tranlate * rotation));
		glUniform4fv(_debugColorLoc, 1, glm::value_ptr(color));

		glBindVertexArray(_debugVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}


	void OpenGLRenderer::DrawCircle(const glm::vec2& position, const glm::vec2& radius, const glm::vec4& color, const glm::mat4& vpMatrix) const
	{
		std::vector<float> vertices;

		double pi = 3.1415926535897932384626433832795;
		for (int a = 0; a < 360; a += 360 / 16)
		{
			float heading = a * (pi / 180.0);
			vertices.push_back(position.x + (cos(heading) * radius.x));
			vertices.push_back(position.y + (sin(heading) * radius.y));
			vertices.push_back(0.0f);
		}

		glBindBuffer(GL_ARRAY_BUFFER, _debugVBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW);

		glUseProgram(_debugShader);
		glUniformMatrix4fv(_debugVPMatLoc, 1, GL_FALSE, glm::value_ptr(vpMatrix));
		glUniformMatrix4fv(_debugModelMatLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
		glUniform4fv(_debugColorLoc, 1, glm::value_ptr(color));

		glBindVertexArray(_debugVAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 16);
		glBindVertexArray(0);
	}
}
