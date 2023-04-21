/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>

/* local includes */
#include "Grid2DGizmo.h"
#include "services/ServiceManager.h"
#include "services/EventsService.h"
#include "services/SettingsService.h"
#include "views/SceneViewer/SceneViewer.h"
#include "events/EditorEvents.h"

/* external includes */
#include <glm/glm.hpp>

namespace Wyrd::Editor
{
	Grid2DGizmo::Grid2DGizmo(SceneViewer* sceneViewer) : Gizmo(sceneViewer, ENTITY_INVALID)
	{
		/* Store the camera controller */
		_CameraController = sceneViewer->GetCamera();

		/* Retrieve the services */
		_EventService = ServiceManager::Get<EventService>();
		_SettingsService = ServiceManager::Get<SettingsService>();

		/* Retrieve settings */
		//_Enabled = Utils::ToBool(_SettingsService->GetSetting(CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__ENABLED, std::string("1")));
		//_Color = Utils::ToColor(_SettingsService->GetSetting(CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__COLOR, std::string("1.0,1.0,1.0,1.0")));

		/* Subscribe to settings events */
		_EventService->Subscribe(Events::EventType::SettingsUpdated, WYRD_BIND_FN(Grid2DGizmo::OnSettingsChanged));

		/* Create and bind a default VAO */
		_VertexArray.reset(VertexArray::Create());
		_VertexArray->Bind();

		BuildGrid();
		
		return;
	}

	Grid2DGizmo::~Grid2DGizmo()
	{
		WYRD_TRACE("GOOEL");
	}

	void Grid2DGizmo::Render(Timestep ts, Renderer& renderer)
	{
		/* calculate the different between the camera viewport and the sceneviewer to set the scalling */
		//float diff = _CameraController->GetSize() / std::min<float>(_SceneViewer->GetViewport()._size.x, _SceneViewer->GetViewport()._size.y);

		//{
		//	Wyrd::DrawVertex2DCommand cmd{};
		//	cmd.type = 1;
		//	cmd.position = Vector2 { 0.0f, 0.0f };
		//	cmd.vertices = &_Vertices;
		//	cmd.vpMatrix = _CameraController->GetCamera().GetViewProjectionMatrix();
		//	cmd.shader = Application::Get().GetResources().Shaders["Vertex2D"].get();
		//	cmd.color = Color { 0.2f, 0.2f, 0.2f, 1.0f };
		//	cmd.drawType = RendererDrawType::Triangles;
		//	
		//	renderer.Submit(cmd);
		//	renderer.Flush();
		//}
		//{
		//	Wyrd::DrawRectCommand cmd{};
		//	cmd.shader = Application::Get().GetResources().Shaders["Vertex2D"].get();
		//	cmd.vpMatrix = _CameraController->GetCamera().GetViewProjectionMatrix();
		//	cmd.position = Vector2 { cursorPosition.x, cursorPosition.y };
		//	cmd.rotationOrigin = Vector2 { 0.0f, 0.0f };
		//	cmd.rotation = 0.0f;
		//	cmd.size = Vector2 { 64.0f, 64.0f };
		//	cmd.thickness = 4.0f;
		//	cmd.color = Color { 0.7f, 0.7f, 0.7f, 1.0f };
		//
		//	renderer.Submit(cmd);
		//	renderer.Flush();
		//}
	}

	void Grid2DGizmo::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseMovedEvent>(WYRD_BIND_EVENT_FN(Grid2DGizmo::OnMouseMovedEvent), nullptr);
		dispatcher.Dispatch<MouseButtonPressedEvent>(WYRD_BIND_EVENT_FN(Grid2DGizmo::OnMouseButtonPressedEvent), nullptr);

	}

	bool Grid2DGizmo::OnMouseMovedEvent(MouseMovedEvent& e, void* data)
	{
		glm::vec4 worldPos = glm::vec4(_SceneViewer->Convert2DToWorldSpace({ e.GetX(), e.GetY() }), -1.0f, 1.0f);

		cursorPosition.x = floor((worldPos.x / 64.0f)) * 64.0f;
		cursorPosition.y = floor((worldPos.y / 64.0f)) * 64.0f;

		return true;
	}

	bool Grid2DGizmo::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e, void* data)
	{
		//glm::vec4 worldPos = glm::vec4(_SceneViewer->Convert2DToWorldSpace({ e.GetPositionX(), e.GetPositionY() }), -1.0f, 1.0f);
		//
		//glm::vec3 cellPos = { floor((worldPos.x / 64.0f)) * 64.0f, floor((worldPos.y / 64.0f)) * 64.0f, 1.0f };
		//
		//std::shared_ptr<Scene> scene = _SceneViewer->GetScene();
		//
		//Entity ent = scene->CreateEntity();
		//scene->AssignComponent<SpriteComponent>(ent);
		//
		//
		//auto transform = scene->Get<Transform2DComponent>(ent);
		//transform->position.x = cellPos.x;
		//transform->position.y = cellPos.y;

		return true;
	}

	void Grid2DGizmo::BuildGrid()
	{
		float gap = 100.0f;
		float thickness = 0.5f;
		
		float xSpacing = 64; // Utils::ToFloat(_SettingsService->GetSetting(CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__XSPACING, std::string("64")));
		float ySpacing = 64; // Utils::ToFloat(_SettingsService->GetSetting(CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__YSPACING, std::string("64")));
		float columnCnt = 128; // Utils::ToFloat(_SettingsService->GetSetting(CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__COLS, std::string("128")));
		float rowCnt = 128; // Utils::ToFloat(_SettingsService->GetSetting(CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__ROWS, std::string("128")));

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
		
		int offset = ((int)columnCnt+1) * 4;
		
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


	void Grid2DGizmo::OnSettingsChanged(Events::EventArgs& args)
	{
		WYRD_TRACE("UPDATE GRID: SETTINGS MAY HAVE CHANGED!!!");
	}
}