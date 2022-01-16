#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>

/* local includes */
#include "GridGizmo.h"
#include "services/ServiceManager.h"
#include "services/EventsService.h"
#include "services/SettingsService.h"
#include "views/SceneViewer/SceneViewer.h"
#include "events/EditorEvents.h"

/* external includes */
#include <glm/glm.hpp>

namespace Wyrd::Editor
{
	GridGizmo::GridGizmo(SceneViewer* sceneViewer) : Gizmo(sceneViewer, ENTITY_INVALID)
	{
		/* Store the camera controller */
		_CameraController = sceneViewer->GetCamera();

		/* Retrieve the services */
		_EventService = ServiceManager::Get<EventService>();
		_SettingsService = ServiceManager::Get<SettingsService>();

		/* Retrieve settings */
		_Enabled = Utils::ToBool(_SettingsService->GetSetting(CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__ENABLED, std::string("1")));
		_Color = Utils::ToColor(_SettingsService->GetSetting(CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__COLOR, std::string("1.0,1.0,1.0,1.0")));

		/* Subscribe to settings events */
		_EventService->Subscribe(Events::EventType::SettingsUpdated, EVENT_FUNC(GridGizmo::OnSettingsChanged));

		/* Create and bind a default VAO */
		_VertexArray.reset(VertexArray::Create());
		_VertexArray->Bind();

		BuildGrid();
		
		return;
	}

	GridGizmo::~GridGizmo() 
	{
		WYRD_TRACE("GOOEL");
	}

	void GridGizmo::Render(Timestep ts, Renderer& renderer)
	{
		/* calculate the different between the camera viewport and the sceneviewer to set the scalling */
		float diff = _CameraController->GetSize() / std::min(_SceneViewer->GetViewport()._size.x, _SceneViewer->GetViewport()._size.y);

		Wyrd::DrawVertex2DCommand cmd{};
		cmd.type = 1;
		cmd.position = { 0.0f, 0.0f };
		cmd.vertices = &_Vertices;
		cmd.vpMatrix = _CameraController->GetCamera().GetViewProjectionMatrix();
		cmd.shader = Application::Get().GetResources().Shaders["Vertex2D"].get();
		cmd.color = { 0.2f, 0.2f, 0.2f, 1.0f };
		cmd.drawType = RendererDrawType::Triangles;

		renderer.Submit(cmd);

		renderer.Flush();
	}

	void GridGizmo::BuildGrid()
	{
		float gap = 100.0f;
		float thickness = 0.5f;
		
		float xSpacing = Utils::ToFloat(_SettingsService->GetSetting(CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__XSPACING, std::string("64")));
		float ySpacing = Utils::ToFloat(_SettingsService->GetSetting(CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__YSPACING, std::string("64")));
		float columnCnt = Utils::ToFloat(_SettingsService->GetSetting(CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__COLS, std::string("128")));
		float rowCnt = Utils::ToFloat(_SettingsService->GetSetting(CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__ROWS, std::string("128")));

		float width = columnCnt * xSpacing;
		float height = rowCnt * ySpacing;
		
		_Vertices.clear();
		
		for (int i = 0; i <= columnCnt; i++)
		{
			_Vertices.push_back({ (xSpacing * i) + -thickness, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f });
			_Vertices.push_back({ (xSpacing * i) + -thickness, width, 1.0f, 1.0f, 1.0f, 1.0f });
			_Vertices.push_back({ (xSpacing * i) + thickness,  width, 1.0f, 1.0f, 1.0f, 1.0f });
			_Vertices.push_back({ (xSpacing * i) + thickness,  width, 1.0f, 1.0f, 1.0f, 1.0f });
			_Vertices.push_back({ (xSpacing * i) + thickness,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f });
			_Vertices.push_back({ (xSpacing * i) + -thickness, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f });
		}
		
		int offset = (columnCnt+1) * 4;
		
		for (int i = 0; i <= rowCnt; i++)
		{
			_Vertices.push_back({ 0.0f, (ySpacing * i) + -thickness,	1.0f, 1.0f, 1.0f, 1.0f });
			_Vertices.push_back({ 0.0f, (ySpacing * i) + thickness,	1.0f, 1.0f, 1.0f, 1.0f });
			_Vertices.push_back({ height,  (ySpacing * i) + thickness,	1.0f, 1.0f, 1.0f, 1.0f });
			_Vertices.push_back({ height,  (ySpacing * i) + thickness,	1.0f, 1.0f, 1.0f, 1.0f });
			_Vertices.push_back({ height,  (ySpacing * i) + -thickness,	1.0f, 1.0f, 1.0f, 1.0f });
			_Vertices.push_back({ 0.0f, (ySpacing * i) + -thickness,	1.0f, 1.0f, 1.0f, 1.0f });
		}
		
		/* Create a new Vertex and Index buffer on the GPU */
		_VertexBuffer.reset(VertexBuffer::Create((float*)&_Vertices[0], _Vertices.size() * sizeof(Vertex2D), "Test VBO"));
		
		/* Setup the Vertex array attribute data */
		_VertexArray->SetAttribute(0, 0, 2, sizeof(Vertex2D));
		_VertexArray->SetAttribute(1, 2, 4, sizeof(Vertex2D));
	}


	void GridGizmo::OnSettingsChanged(Events::EventArgs& args)
	{
		WYRD_TRACE("UPDATE GRID: SETTINGS MAY HAVE CHANGED!!!");
	}
}