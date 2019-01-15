#pragma once

#include "osrpch.h"
#include "OpenGLRenderer.h"

#include <glad/glad.h>

namespace Osiris {

	OpenGLRenderer::OpenGLRenderer()
	{
		OSR_CORE_TRACE("Renderer Created");
	}

	Renderer* Renderer::Create()
	{
		return new OpenGLRenderer();
	}

	OpenGLRenderer::~OpenGLRenderer()
	{

	}

	unsigned int OpenGLRenderer::LoadShader(std::shared_ptr<Shader> shader)
	{
		unsigned int program = glCreateProgram();
		unsigned int vs = CompileShader(GL_VERTEX_SHADER, shader->VertexSrc.c_str());
		unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, shader->FragmentSrc.c_str());

		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);

		glDetachShader(program, vs);
		glDetachShader(program, fs);

		glDeleteShader(vs);
		glDeleteShader(fs);

		return program;
	}

	unsigned int OpenGLRenderer::LoadMesh(std::unique_ptr<Mesh>& mesh)
	{
		// Generate 1 buffer, put the resulting identifier in vertexbuffer
		glGenBuffers(1, &mesh->vbo);
		// The following commands will talk about our 'vertexbuffer' buffer
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
		// Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh->vertices.size(), &mesh->vertices[0], GL_STATIC_DRAW);
		
		return 1;
	}


	unsigned int OpenGLRenderer::RenderMesh(std::unique_ptr<Mesh>& mesh)
	{
		/* 1st attribute buffer : vertices */
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			4,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		/* Draw the triangle ! */
		glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.size()); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);

		return 0;
	}


	unsigned int OpenGLRenderer::CompileShader(unsigned int type, const std::string& source)
	{
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);
			OSR_CORE_ERROR(message);
			glDeleteShader(id);
			return 0;
		}

		return id;
	}
}
