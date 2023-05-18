/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>

/* local includes */
#include "Grid3DGizmo.h"
#include "services/ServiceManager.h"
#include "services/EventsService.h"
#include "services/SettingsService.h"
#include "views/SceneViewer/SceneViewer.h"
#include "events/EditorEvents.h"

/* external includes */
#include <glm/glm.hpp>

namespace Wyrd::Editor
{
	Grid3DGizmo::Grid3DGizmo(SceneViewer* sceneViewer) : Gizmo(sceneViewer, ENTITY_INVALID)
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
		_EventService->Subscribe(Events::EventType::SettingsUpdated, WYRD_BIND_FN(Grid3DGizmo::OnSettingsChanged));

		/* Create and bind a default VAO */
		_VertexArray.reset(VertexArray::Create());
		_VertexArray->Bind();

		BuildGrid();
		
		return;
	}

	Grid3DGizmo::~Grid3DGizmo()
	{
		WYRD_TRACE("GOOEL");
	}

	void Grid3DGizmo::Render(Timestep ts, Renderer& renderer)
	{
		{
			Wyrd::DrawVertex3DCommand cmd{};
			cmd.type = 1;
			cmd.position = Vector3 { 0.0f, 0.0f, 0.0f };
			cmd.vertices = &_gridVertices;
			cmd.viewMatrix = _CameraController->GetCamera().GetViewMatrix();
			cmd.projectionMatrix = _CameraController->GetCamera().GetProjectionMatrix();
			cmd.material = Application::Get().GetResources().Materials[RES_MATERIAL_GRID3D].get();
			cmd.color = Color { 0.2f, 0.2f, 0.2f, 1.0f };
			cmd.drawType = RendererDrawType::Triangles;
			
			renderer.Submit(cmd);
			renderer.Flush();
		}
	}

	void Grid3DGizmo::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseMovedEvent>(WYRD_BIND_EVENT_FN(Grid3DGizmo::OnMouseMovedEvent), nullptr);
		dispatcher.Dispatch<MouseButtonPressedEvent>(WYRD_BIND_EVENT_FN(Grid3DGizmo::OnMouseButtonPressedEvent), nullptr);

	}

	bool Grid3DGizmo::OnMouseMovedEvent(MouseMovedEvent& e, void* data)
	{
		glm::vec4 worldPos = glm::vec4(_SceneViewer->Convert2DToWorldSpace({ e.GetX(), e.GetY() }), -1.0f, 1.0f);

		cursorPosition.x = floor((worldPos.x / 64.0f)) * 64.0f;
		cursorPosition.y = floor((worldPos.y / 64.0f)) * 64.0f;

		return true;
	}

	bool Grid3DGizmo::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e, void* data)
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

	void Grid3DGizmo::BuildGrid()
	{		
		_gridVertices.clear();

		_gridVertices.push_back({ 1.0f, 1.0f, 0.0f });
		_gridVertices.push_back({ -1.0f, -1.0f, 0.0f });
		_gridVertices.push_back({ -1.0f, 1.0f, 0.0f });
		_gridVertices.push_back({ -1.0f, -1.0f, 0.0f });
		_gridVertices.push_back({ 1.0f, 1.0f, 0.0f });
		_gridVertices.push_back({ 1.0f, -1.0f, 0.0f });
		
		/* Create a new Vertex and Index buffer on the GPU */
		_VertexBuffer.reset(VertexBuffer::Create((float*)&_gridVertices[0], _gridVertices.size() * sizeof(Vertex3D), "Test VBO"));
		
		/* Setup the Vertex array attribute data */
		_VertexArray->SetAttribute(0, 0, 3, sizeof(Vertex3D));
		//_VertexArray->SetAttribute(1, 3, 2, sizeof(Vertex2D));
	}


	void Grid3DGizmo::OnSettingsChanged(Events::EventArgs& args)
	{
		WYRD_TRACE("UPDATE GRID: SETTINGS MAY HAVE CHANGED!!!");
	}
}