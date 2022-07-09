/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/export.h>
#include <core/Timestep.h>
#include <core/Application.h>
#include <core/support/PrimitiveUtils.h>

/* local includes */
#include "Translation2DGizmo.h"
#include "views/SceneViewer/SceneViewer.h"
#include "services/ServiceManager.h"
#include "services/EventsService.h"
#include "events/EditorEvents.h"
#include "loaders/ObjLoader.h"
#include "datamodels/EditorComponents.h"

/* external includes */
#include <glm/glm.hpp>

namespace Wyrd::Editor
{
    Translation2DGizmo::Translation2DGizmo(SceneViewer* sceneViewer) : Gizmo(sceneViewer, ENTITY_INVALID), _XYMoveActive(false)
	{
		/* retrieve the services */
		_EventService = ServiceManager::Get<EventService>();

		/* setup event subscriptions */
		_EventService->Subscribe(Events::EventType::SelectedEntityChanged, WYRD_BIND_FN(Translation2DGizmo::OnSelectedEntityChanged));
	}

    Translation2DGizmo::~Translation2DGizmo() {}

	void Translation2DGizmo::Render(Timestep ts, Renderer& renderer)
	{
        if (_Entity != ENTITY_INVALID)
        {
            /* calculate the different between the camera viewport and the sceneviewer to set the scalling */
            float diff = _CameraController->GetSize() / std::min(_SceneViewer->GetViewport()._size.x, _SceneViewer->GetViewport()._size.y);

            //Transform2DComponent* transform2DComponent = _SceneViewer->GetScene()->Get<Transform2DComponent>(_Entity);
            //
            //Wyrd::DrawVertex2DCommand cmd{};
            //cmd.type = 1;
            //cmd.position = transform2DComponent->position;
            //cmd.vertices = &_Vertices;
            //cmd.vpMatrix = _CameraController->GetCamera().GetViewProjectionMatrix();
            //cmd.shader = Application::Get().GetResources().Shaders["Vertex2D"].get();
            //cmd.color = { 1.0f, 1.0f, 1.0f, 1.0f };
            //cmd.drawType = RendererDrawType::Triangles;
            //
            //Build();
            //
            //renderer.Submit(cmd);
        }
	}

	void Translation2DGizmo::OnSelectedEntityChanged(Events::EventArgs& args)
	{
		Events::SelectedEntityChangedArgs& evtArgs = static_cast<Events::SelectedEntityChangedArgs&>(args);

        _Entity = evtArgs.entity;

        Build();
	}

    void Translation2DGizmo::Build()
    {
        EditorComponent* editorComponent = _SceneViewer->GetScene()->Get<EditorComponent>(_Entity);
        //Transform2DComponent* transform2DComponent = _SceneViewer->GetScene()->Get<Transform2DComponent>(_Entity);
        //if (editorComponent != nullptr && transform2DComponent != nullptr)
        //{
        //    /* create the current set of vertices */
        //    _Vertices.clear();
        //
        //    /* pivot position */
        //    glm::vec2 pivot = transform2DComponent->rotationOrigin;
        //
        //    uint32_t offset = 0;
        //
        //    /* entity outline */
        //    offset = PrimitiveUtils::GenerateRect(_Vertices, offset, { 0.0f, 0.0f }, editorComponent->inputArea._size, _XYMoveActive ? Color::MAGENTA : Color::CYAN, 2.0f, transform2DComponent->rotation, pivot);
        //
        //    /* axis movement*/
        //    offset = PrimitiveUtils::GenerateRect(_Vertices, offset, { 10.0f, -2.0f }, { 20.0f, 4.0f }, Color::RED, 2.0f, transform2DComponent->rotation, pivot);
        //    offset = PrimitiveUtils::GenerateRect(_Vertices, offset, { -2.0f, 10.0f }, { 4.0f, 20.0f }, Color::GREEN, 2.0f, transform2DComponent->rotation, pivot);
        //    
        //    /* origin */
        //    offset = PrimitiveUtils::GenerateLine(_Vertices, offset, { -10.0f, 0.0f }, { 10.0f, 0.0f }, Color::WHITE, 2.0f);
        //    offset = PrimitiveUtils::GenerateLine(_Vertices, offset, { 0.0f, -10.0f }, { 0.0f, 10.0f }, Color::WHITE, 2.0f);
        //
        //    /* rotation pivot */
        //    offset = PrimitiveUtils::GenerateCircle(_Vertices, offset, transform2DComponent->rotationOrigin, 5.0f, 8.0f, Color::YELLOW, 2.0f);
        //    offset = PrimitiveUtils::GenerateVector2D(_Vertices, offset, transform2DComponent->rotationOrigin, transform2DComponent->rotation, 20.0f, Color::YELLOW, 2.0f);
        //
        //    /* Temp Handles */
        //    offset = PrimitiveUtils::GenerateRect(_Vertices, offset, { 32.0f, 32.0f }, { 32.0f, 32.0f }, Color::MAGENTA, 2.0f);
        //    offset = PrimitiveUtils::GenerateRect(_Vertices, offset, { 64.0f, 32.0f }, { 32.0f, 32.0f }, Color::MAGENTA, 2.0f);
        //
        //
        //    /* create the current set of vertices */
        //    _Handles.clear();
        //    _Handles.reserve(1);
        //
        //    GizmoHandle& moveHandle = CreateHandle({ 0.0f, 0.0f }, editorComponent->inputArea._size, this, transform2DComponent->CalculateModelMatrix());
        //
        //    moveHandle.SetMouseButtonPressedCallback([=](MouseButtonPressedEvent& evt)
        //    {   
        //        _XYMoveActive = true;
        //        return true; 
        //    });
        //
        //    moveHandle.SetMouseButtonReleaseCallback([=](MouseButtonReleasedEvent& evt)
        //    {
        //        _XYMoveActive = false;
        //        return true;
        //    });
        //}
    }
}