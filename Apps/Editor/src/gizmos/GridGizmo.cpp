#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/Log.h>
#include <core/scene/components/Transform2DComponent.h>

/* local includes */
#include "GridGizmo.h"
#include "services/ServiceManager.h"

/* external includes */
#include <glm/glm.hpp>

namespace Osiris::Editor
{
	struct GridVertex
	{
		float x, y, u, v;
	};

	GridGizmo::GridGizmo(std::shared_ptr<OrthographicCameraController> cameraController)
	{
		/* Store the camera controller */
		_CameraController = cameraController;

		/* Retrieve the shader */
		_Shader = Application::Get().GetResources().Shaders["GridGizmo"];

		///* Create and bind a default VAO */
		_VertexArray.reset(VertexArray::Create());
		_VertexArray->Bind();

		///* define data for a simple rectangle with matching indices */
		float gap = 100.0f;
		std::vector<GridVertex> vertices;
		std::vector<uint32_t> indices;
		for (int i = 0; i <= 10; i++)
		{
			vertices.push_back({ (gap * i) + -2.0f, -1.0f,	 0.0f, 0.0f });
			vertices.push_back({ (gap * i) + -2.0f, 1000.0f, 0.0f, 0.0f });
			vertices.push_back({ (gap * i) + 2.0f,  1000.0f, 0.0f, 0.0f });
			vertices.push_back({ (gap * i) + 2.0f,  -1.0f,	 0.0f, 0.0f });

			indices.push_back((i * 4) + 0);
			indices.push_back((i * 4) + 1);
			indices.push_back((i * 4) + 2);
			indices.push_back((i * 4) + 2);
			indices.push_back((i * 4) + 3);
			indices.push_back((i * 4) + 0);
		}

		for (int i = 0; i <= 10; i++)
		{
			vertices.push_back({ -2.0f, (gap * i) + -1.0f,	0.0f, 0.0f });
			vertices.push_back({ -2.0f, (gap * i) + 1.0f,	0.0f, 0.0f });
			vertices.push_back({ 1000.0f,  (gap * i) + 1.0f,	0.0f, 0.0f });
			vertices.push_back({ 1000.0f,  (gap * i) + -1.0f,	0.0f, 0.0f });

			indices.push_back(44 + (i * 4) + 0);
			indices.push_back(44 + (i * 4) + 1);
			indices.push_back(44 + (i * 4) + 2);
			indices.push_back(44 + (i * 4) + 2);
			indices.push_back(44 + (i * 4) + 3);
			indices.push_back(44 + (i * 4) + 0);
		}

		/* Create a new Vertex and Index buffer on the GPU */
		_VertexBuffer.reset(VertexBuffer::Create((float*)&vertices[0], vertices.size() * sizeof(GridVertex), "Test VBO"));
		_IndexBuffer.reset(IndexBuffer::Create(&indices[0], indices.size()));

		/* Setup the Vertex array attribute data */
		_VertexArray->SetAttribute(0, 0, 2);

		/* bind the shader */
		_Shader->Bind();

		/* set the vp matrix to a standard otho matrix */
		_Shader->SetVPMatrix(glm::ortho(0.0f, _CameraController->GetCamera().GetRight(), 0.0f, _CameraController->GetCamera().GetTop()));

		/* set the blend color */
		_Shader->SetUniformVec3("blendColor", glm::vec3{ 0.2f, 0.2f, 0.2f });

		return;
	}

	GridGizmo::~GridGizmo() {}

	void GridGizmo::Render(Timestep ts, Renderer& renderer)
	{
		_Shader->Bind();
		_VertexBuffer->Bind();
		_IndexBuffer->Bind(); 
		_VertexArray->Bind();
				
		_Shader->SetVPMatrix(_CameraController->GetCamera().GetViewProjectionMatrix());
		_Shader->SetMatrix("model", glm::mat4(1.0f));

		renderer.DrawElements(RendererDrawType::Triangles, _IndexBuffer->GetCount());

	}
}