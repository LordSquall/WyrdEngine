#include "OGLRenderer.h"


#define GLM_ENABLE_EXPERIMENTAL

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>

using namespace OrisisEngine;

OGLRenderer::OGLRenderer()
{
}

OGLRenderer::OGLRenderer(const OGLRenderer &obj)
{
}

OGLRenderer::~OGLRenderer()
{
}

void OGLRenderer::RegisterLogger(ILogger* logger)
{
	_logger = logger;
}

ILogger* OGLRenderer::GetLogger()
{
	return _logger;
}

bool OGLRenderer::Initialise(IWindow* window)
{
	_logger->LogInfo("OGLRenderer: Initialise Called");
	return true;
}

void OGLRenderer::Shutdown()
{
}

bool OGLRenderer::CreateTexture(Texture* texture)
{
	GLuint handle;
	glGenTextures(1, &handle);
	texture->Handle(handle);

	_logger->LogDebug("OGL Texture generated with handle: " + to_string(texture->Handle()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->Handle());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->Width(), texture->Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->PixelData());
	glGenerateMipmap(GL_TEXTURE_2D);


	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

bool OGLRenderer::UpdateTexture(Texture* texture)
{

	return true;
}

bool OGLRenderer::SetTexture(Texture* texture)
{
	glBindTexture(GL_TEXTURE_2D, texture->Handle());

	return true;
}

bool OGLRenderer::RegisterShader(Shader* shader)
{
	int IsCompiled_VS, IsCompiled_FS, IsLinked;
	int maxLength;
	char *vertexInfoLog;
	char *fragmentInfoLog;
	char *shaderProgramInfoLog;

	shader->VertexHandle = glCreateShader(GL_VERTEX_SHADER);
	const char *vertex_str = shader->VertexSource.c_str();
	glShaderSource(shader->VertexHandle, 1, &vertex_str, 0);
	glCompileShader(shader->VertexHandle);

	glGetShaderiv(shader->VertexHandle, GL_COMPILE_STATUS, &IsCompiled_VS);
	if (IsCompiled_VS == false)
	{
		glGetShaderiv(shader->VertexHandle, GL_INFO_LOG_LENGTH, &maxLength);
		vertexInfoLog = (char *)malloc(maxLength);
		glGetShaderInfoLog(shader->VertexHandle, maxLength, &maxLength, vertexInfoLog);
		_logger->LogError("Shader [" + shader->VertexFileName + "]: " + vertexInfoLog);
		free(vertexInfoLog);
		return false;
	}

	shader->FragmentHandle = glCreateShader(GL_FRAGMENT_SHADER);
	const char *fragment_str = shader->FragmentSource.c_str();
	glShaderSource(shader->FragmentHandle, 1, &fragment_str, 0);
	glCompileShader(shader->FragmentHandle);
	glGetShaderiv(shader->FragmentHandle, GL_COMPILE_STATUS, &IsCompiled_FS);
	if (IsCompiled_FS == false)
	{
		glGetShaderiv(shader->FragmentHandle, GL_INFO_LOG_LENGTH, &maxLength);
		fragmentInfoLog = (char *)malloc(maxLength);
		glGetShaderInfoLog(shader->FragmentHandle, maxLength, &maxLength, fragmentInfoLog);
		_logger->LogError("Shader [" + shader->FragmentFileName + "]: " + fragmentInfoLog);
		free(fragmentInfoLog);
		return false;
	}

	shader->Handle = glCreateProgram();

	glAttachShader(shader->Handle, shader->VertexHandle);
	glAttachShader(shader->Handle, shader->FragmentHandle);

	glBindAttribLocation(shader->Handle, 0, "in_Position");
	glBindAttribLocation(shader->Handle, 1, "in_TextureUV");

	glLinkProgram(shader->Handle);

	glGetProgramiv(shader->Handle, GL_LINK_STATUS, (int *)&IsLinked);
	if (IsLinked == false)
	{
		glGetProgramiv(shader->Handle, GL_INFO_LOG_LENGTH, &maxLength);
		shaderProgramInfoLog = (char *)malloc(maxLength);
		glGetProgramInfoLog(shader->Handle, maxLength, &maxLength, shaderProgramInfoLog);
		_logger->LogError("Shader [LINKER]: " + string(shaderProgramInfoLog));
		free(shaderProgramInfoLog);
		return false;
	}

	glUseProgram(shader->Handle);
}

bool OGLRenderer::UseShader(Shader* shader)
{
	return true;
}

bool OGLRenderer::CreateVertexArray(VertexArray* vertexArray)
{
	glGenVertexArrays(1, &vertexArray->Handle);
	_logger->LogDebug("OGL Vertex Array generated with handle: " + to_string(vertexArray->Handle));

	glBindVertexArray(vertexArray->Handle);

	return true;
}

bool OGLRenderer::DeleteVertexArray(VertexArray* vertexArray)
{
	return true;
}

bool OGLRenderer::CreateVertexBuffer(VertexBuffer* vertexBuffer)
{
	glGenBuffers(1, &vertexBuffer->Handle);
	_logger->LogDebug("OGL Vertex Buffer generated with handle: " + to_string(vertexBuffer->Handle));

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->Handle);
	return true;
}

bool OGLRenderer::DeleteVertexBuffer(VertexBuffer* vertexBuffer)
{
	return true;
}

bool OGLRenderer::UpdateVertexBuffer(VertexBuffer* vertexBuffer, unsigned int offset, unsigned int size)
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->Handle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * size, &vertexBuffer->GetArray()[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 2));

	return true;
}


bool OGLRenderer::DrawVertexBuffer(VertexBuffer* vertexBuffer)
{
	/* TEMP */
	GLuint mvpLoc = glGetUniformLocation(3, "u_mvp");

	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 View = glm::mat4(1.0f);
	glm::mat4 Projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 100.0f);

	glm::mat4 MVP = Projection;// *View * Model;

	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &MVP[0][0]);

	GLuint texture = glGetUniformLocation(3, "u_texture");

	glUniform1i(texture, 0);

	glDrawArrays(GL_TRIANGLES, 0, vertexBuffer->GetCount());

	return true;
}