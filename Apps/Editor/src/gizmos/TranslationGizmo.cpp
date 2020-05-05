#pragma once

#include "osrpch.h"

#include "TranslationGizmo.h"

namespace Osiris::Editor
{
	TranslationGizmo::TranslationGizmo() 
	{
		/* Load gizmo shader */
		std::ifstream vertexStream("res/shaders/gizmo.vert");
		std::string vertexSrc((std::istreambuf_iterator<char>(vertexStream)), std::istreambuf_iterator<char>());

		std::ifstream fragmentStream("res/shaders/gizmo.frag");
		std::string fragmentSrc((std::istreambuf_iterator<char>(fragmentStream)), std::istreambuf_iterator<char>());

		_Shader.reset(Shader::Create());

		if (_Shader->Build(vertexSrc, fragmentSrc) == false)
		{
			OSR_ERROR("Unable to build shader.");
		}
		_Shader->Bind();

		/* Set the shaders uniforms  */
		_Shader->SetUniformVec3("blendColor", glm::vec3{ 0.8f, 0.2f, 0.2f });

		/* Create and bind a default VAO */
		_VertexArray.reset(VertexArray::Create());
		_VertexArray->Bind();

		/* define data for a simple rectangle with matching indices */
		float vertices[16] =
		{
			-2.0f, -2.0f, 0.0f, 0.0f, 
			-2.0f,  2.0f, 0.0f, 0.0f,
			2.0f,   2.0f, 0.0f, 0.0f,
			2.0f,  -2.0f, 0.0f, 0.0f
		};

		uint32_t indices[6] =
		{
			0, 1, 2, 2, 3, 0
		};

		/* Create a new Vertex and Index buffer on the GPU */
		_VertexBuffer.reset(VertexBuffer::Create(&vertices[0], sizeof(vertices), "Test VBO"));
		_IndexBuffer.reset(IndexBuffer::Create(&indices[0], sizeof(indices) / sizeof(uint32_t)));

		/* Setup the Vertex array attribute data */
		_VertexArray->SetAttribute(0, 0, 2);
	}

	TranslationGizmo::~TranslationGizmo() {}

	void TranslationGizmo::SetCameraController(std::shared_ptr<OrthographicCameraController> cameraController)
	{
		_CameraController = cameraController;
	}

	void TranslationGizmo::SetGameObject(std::shared_ptr<GameObject> gameObject)
	{
		_GameObject = gameObject;
	}

	void TranslationGizmo::Render(Timestep ts, Renderer& renderer)
	{
		_Shader->Bind();
		_VertexBuffer->Bind();
		_IndexBuffer->Bind(); 
		_VertexArray->Bind();

		_Shader->SetUniformVec2("positionOffset", _GameObject->transform2d->position);
		_Shader->SetVPMatrix(_CameraController->GetCamera().GetViewProjectionMatrix());

		renderer.DrawElements(RendererDrawType::Triangles, _IndexBuffer->GetCount());
	}
}