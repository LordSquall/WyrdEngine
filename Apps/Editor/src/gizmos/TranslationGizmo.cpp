#pragma once

#include "osrpch.h"

#include "TranslationGizmo.h"

#include "services/ServiceManager.h"
#include "datamodels/components/Transform2DComponent.h"

namespace Osiris::Editor
{
	TranslationGizmo::TranslationGizmo(std::shared_ptr<Shader> shader, std::shared_ptr<OrthographicCameraController> cameraController)
	{
		/* Store the shader and controller */
		_Shader = shader;
		_CameraController = cameraController;

		/* retrive textures and icons */
		_Icon = ServiceManager::Get<ResourceService>(ServiceManager::Service::Resources)->GetIconLibrary().GetIcon(std::string("gizmos"), std::string("gizmo_translate2D"));

		/* Create and bind a default VAO */
		_VertexArray.reset(VertexArray::Create());
		_VertexArray->Bind();

		/* define data for a simple rectangle with matching indices */
		float vertices[16] =
		{
			-2.0f, -2.0f, _Icon->uv[0].x, _Icon->uv[0].y,
			-2.0f,  2.0f, _Icon->uv[1].x, _Icon->uv[1].y,
			2.0f,   2.0f, _Icon->uv[2].x, _Icon->uv[2].y,
			2.0f,  -2.0f, _Icon->uv[3].x, _Icon->uv[3].y
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
		_VertexArray->SetAttribute(1, 2, 2);

		return;
	}

	TranslationGizmo::~TranslationGizmo() {}

	void TranslationGizmo::SetGameObject(std::shared_ptr<GameObject> gameObject)
	{
		_GameObject = gameObject;
	}

	void TranslationGizmo::OnDrag(glm::vec2 delta)
	{
		_GameObject->transform2d.Translate(delta);
	}

	void TranslationGizmo::Render(Timestep ts, Renderer& renderer)
	{
		_Shader->Bind();
		_VertexBuffer->Bind();
		_IndexBuffer->Bind(); 
		_VertexArray->Bind();

		(*_Icon->iconSet->Texture->GetTexture())->Bind();

		_Shader->SetUniformVec2("positionOffset", _GameObject->transform2d.position);
		_Shader->SetVPMatrix(_CameraController->GetCamera().GetViewProjectionMatrix());

		renderer.DrawElements(RendererDrawType::Triangles, _IndexBuffer->GetCount());
	}
}