#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/scene/components/Transform2DComponent.h>

/* local includes */
#include "TranslationGizmo.h"
#include "services/ServiceManager.h"

/* external includes */
#include <glm/glm.hpp>

namespace Osiris::Editor
{
	TranslationGizmo::TranslationGizmo(std::shared_ptr<Shader> shader, std::shared_ptr<OrthographicCameraController> cameraController)
	{
		/* Store the shader and controller */
		_Shader = shader;
		_CameraController = cameraController;

		/* retrive textures and icons */
		_Icon = ServiceManager::Get<ResourceService>(ServiceManager::Service::Resources)->GetIconLibrary().GetIcon(std::string("common"), std::string("gizmo_translate"));

		/* Create and bind a default VAO */
		_VertexArray.reset(VertexArray::Create());
		_VertexArray->Bind();

		/* define data for a simple rectangle with matching indices */
		float vertices[16] =
		{
			-1.0f, -1.0f, _Icon->uv[0].x, _Icon->uv[0].y,
			-1.0f,  1.0f, _Icon->uv[1].x, _Icon->uv[1].y,
			1.0f,   1.0f, _Icon->uv[2].x, _Icon->uv[2].y,
			1.0f,  -1.0f, _Icon->uv[3].x, _Icon->uv[3].y
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

		/* bind the shader */
		_Shader->Bind();

		/* set the vp matrix to a standard otho matrix */
		_Shader->SetVPMatrix(glm::ortho(0.0f, _CameraController->GetCamera().GetRight(), 0.0f, _CameraController->GetCamera().GetTop()));

		/* set the blend color */
		_Shader->SetUniformVec3("blendColor", glm::vec3{ 1.0f, 1.0f, 1.0f });

		return;
	}

	TranslationGizmo::~TranslationGizmo() {}

	void TranslationGizmo::SetGameObject(std::shared_ptr<GameObject> gameObject)
	{
		_GameObject = gameObject;
	}

	void TranslationGizmo::OnDrag(glm::vec2 delta)
	{
		_GameObject->transform2D->Translate(delta);
	}

	void TranslationGizmo::Render(Timestep ts, Renderer& renderer)
	{
		float scaleFactor = _CameraController->GetZoomLevel() / _CameraController->GetAspectRatio();

		_Shader->Bind();
		_VertexBuffer->Bind();
		_IndexBuffer->Bind(); 
		_VertexArray->Bind();

		_Icon->iconSet->Texture->GetTexture()->Bind();
		
		_Shader->SetVPMatrix(_CameraController->GetCamera().GetViewProjectionMatrix());
		_Shader->SetMatrix("model", _GameObject->transform2D->matrix * glm::scale(glm::vec3(8.0f)));

		renderer.DrawElements(RendererDrawType::Triangles, _IndexBuffer->GetCount());
	}
}