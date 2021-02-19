#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/Log.h>
#include <core/scene/components/Transform2DComponent.h>

/* local includes */
#include "GridGizmo.h"
#include "services/ServiceManager.h"
#include "services/EventsService.h"
#include "services/SettingsService.h"
#include "events/EditorEvents.h"

/* external includes */
#include <glm/glm.hpp>

namespace Osiris::Editor
{

	GridGizmo::GridGizmo(std::shared_ptr<OrthographicCameraController> cameraController)
	{
		/* Store the camera controller */
		_CameraController = cameraController;

		/* Retrieve the services */
		_EventService = ServiceManager::Get<EventService>(ServiceManager::Events);
		_SettingsService = ServiceManager::Get<SettingsService>(ServiceManager::Settings);

		/* Retrieve settings */
		_Enabled = Utils::ToBool(_SettingsService->GetSetting(CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__ENABLED, std::string("1")));
		_Color = Utils::ToColor(_SettingsService->GetSetting(CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__COLOR, std::string("1.0,1.0,1.0,1.0")));

		/* Subscribe to settings events */
		_EventService->Subscribe(Events::EventType::SettingsUpdated, EVENT_FUNC(GridGizmo::OnSettingsChanged));

		/* Retrieve the shader */
		_Shader = Application::Get().GetResources().Shaders["GridGizmo"];

		/* Create and bind a default VAO */
		_VertexArray.reset(VertexArray::Create());
		_VertexArray->Bind();

		BuildGrid();


		return;
	}

	GridGizmo::~GridGizmo() 
	{
		OSR_TRACE("GOOEL");
	}

	void GridGizmo::Render(Timestep ts, Renderer& renderer)
	{
		_Shader->Bind();
		_VertexBuffer->Bind();
		_IndexBuffer->Bind(); 
		_VertexArray->Bind();
				
		_Shader->SetVPMatrix(_CameraController->GetCamera().GetViewProjectionMatrix());
		_Shader->SetMatrix("model", glm::mat4(1.0f));

		/* set the blend color */
		_Shader->SetUniformVec4("blendColor", glm::vec4{ _Color.r, _Color.g, _Color.b, _Color.a });

		renderer.DrawElements(RendererDrawType::Triangles, _IndexBuffer->GetCount());

	}

	void GridGizmo::BuildGrid()
	{
		float gap = 100.0f;

		float xSpacing = Utils::ToFloat(_SettingsService->GetSetting(CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__XSPACING, std::string("128")));
		float ySpacing = Utils::ToFloat(_SettingsService->GetSetting(CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__YSPACING, std::string("128")));
		float columnCnt = Utils::ToFloat(_SettingsService->GetSetting(CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__COLS, std::string("128")));
		float rowCnt = Utils::ToFloat(_SettingsService->GetSetting(CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__ROWS, std::string("128")));
		
		float width = columnCnt * xSpacing;
		float height = rowCnt * ySpacing;

		_Vertices.clear();
		_Indices.clear();

		for (int i = 0; i <= columnCnt; i++)
		{
			_Vertices.push_back({ (xSpacing * i) + -2.0f, -1.0f,	 0.0f, 0.0f });
			_Vertices.push_back({ (xSpacing * i) + -2.0f, width, 0.0f, 0.0f });
			_Vertices.push_back({ (xSpacing * i) + 2.0f,  width, 0.0f, 0.0f });
			_Vertices.push_back({ (xSpacing * i) + 2.0f,  -1.0f,	 0.0f, 0.0f });

			_Indices.push_back((i * 4) + 0);
			_Indices.push_back((i * 4) + 1);
			_Indices.push_back((i * 4) + 2);
			_Indices.push_back((i * 4) + 2);
			_Indices.push_back((i * 4) + 3);
			_Indices.push_back((i * 4) + 0);
		}

		int offset = (columnCnt+1) * 4;

		for (int i = 0; i <= rowCnt; i++)
		{
			_Vertices.push_back({ -2.0f, (ySpacing * i) + -1.0f,	0.0f, 0.0f });
			_Vertices.push_back({ -2.0f, (ySpacing * i) + 1.0f,	0.0f, 0.0f });
			_Vertices.push_back({ height,  (ySpacing * i) + 1.0f,	0.0f, 0.0f });
			_Vertices.push_back({ height,  (ySpacing * i) + -1.0f,	0.0f, 0.0f });

			_Indices.push_back(offset + (i * 4) + 0);
			_Indices.push_back(offset + (i * 4) + 1);
			_Indices.push_back(offset + (i * 4) + 2);
			_Indices.push_back(offset + (i * 4) + 2);
			_Indices.push_back(offset + (i * 4) + 3);
			_Indices.push_back(offset + (i * 4) + 0);
		}

		/* Create a new Vertex and Index buffer on the GPU */
		_VertexBuffer.reset(VertexBuffer::Create((float*)&_Vertices[0], _Vertices.size() * sizeof(GridVertex), "Test VBO"));
		_IndexBuffer.reset(IndexBuffer::Create(&_Indices[0], _Indices.size()));

		/* Setup the Vertex array attribute data */
		_VertexArray->SetAttribute(0, 0, 2);

		/* bind the shader */
		_Shader->Bind();

		/* set the vp matrix to a standard otho matrix */
		_Shader->SetVPMatrix(_CameraController->GetCamera().GetViewProjectionMatrix());
	}


	void GridGizmo::OnSettingsChanged(Events::EventArgs& args)
	{
		OSR_TRACE("UPDATE GRID: SETTINGS MAY HAVE CHANGED!!!");
	}
}