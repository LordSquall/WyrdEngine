/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/export.h>
#include <core/Timestep.h>
#include <core/Application.h>

/* local includes */
#include "TranslationGizmo.h"
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

#define HANDLE_XY "XY_Handle"
#define HANDLE_X "X_Handle"
#define HANDLE_Y "Y_Handle"

    static Color colorYAxisNormal = { 0.2f, 0.8f, 0.2f, 1.0f };
    static Color colorYAxisOver = { 0.0f, 1.0f, 0.0f, 1.0f };
    static Color colorXAxisNormal = { 0.8f, 0.2f, 0.2f, 1.0f };
    static Color colorXAxisOver = { 1.0f, 0.0f, 0.0f, 1.0f };
    static Color colorXYAxisNormal = { 0.5f, 0.5f, 0.5f, 1.0f };
    static Color colorXYAxisOver = { 0.2f, 0.2f, 0.2f, 1.0f };

	//TEMP
#define INSIDE 0
#define OUTSIDE 1

    struct Point
    {
        float x;
        float y;
    };

    bool InsidePolygon(std::vector<Vertex2D>& vertices, std::pair<uint32_t, uint32_t>& range, Point p)//Point* polygon, int N, Point p)
    {
        float precision = 0.00001f;
        int counter = 0;
        int i;
        float xinters;
        Vertex2D p1, p2;
        int n = range.second - range.first;

        p1 = vertices[range.first];
        for (i = 0; i <= n; i++) {

            uint32_t next = range.first + (i + 1);
            if (next > range.second)
                next = range.first;

            p2 = vertices[next];
            if (p.y > std::min<float>(p1.y, p2.y)) {
                if (p.y <= std::max<float>(p1.y, p2.y)) {
                    if (p.x <= std::max<float>(p1.x, p2.x)) {
                        if (p1.y != p2.y) {
                            xinters = (p.y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y) + p1.x;
                            if ((fabs(p1.x - p2.x) < precision) || p.x <= xinters)
                                counter++;
                        }
                    }
                }
            }

            p1 = p2;
        }

        if (counter % 2 == 0)
            return false;
        else
            return true;
    }

    TranslationGizmo::TranslationGizmo(SceneViewer* sceneViewer) : Gizmo(sceneViewer, ENTITY_INVALID), _InputState(InputState::NONE), _AxisState(AxisState::XY), _MovementState(MovementState::LOCAL), _LastMouseWorldPos(0.0f, 0.0f)
	{
		/* retrieve the services */
		_EventService = ServiceManager::Get<EventService>();

		/* setup event subscriptions */
		_EventService->Subscribe(Events::EventType::SelectedEntityChanged, WYRD_BIND_FN(TranslationGizmo::OnSelectedEntityChanged));

        /* load the vertices from an external source file */
		ObjLoader::Load(Utils::GetEditorResFolder() + "gizmos\\translation_gizmo.obj", _Vertices, _Indices, _VertexGroups, 0.4f);

		/* update handle colors */
		UpdateVertexGroupColor("XY_Handle", colorXYAxisNormal);
		UpdateVertexGroupColor("Y_Handle", colorYAxisNormal);
		UpdateVertexGroupColor("X_Handle", colorXAxisNormal);
	}

	TranslationGizmo::~TranslationGizmo() {}

	void TranslationGizmo::Render(Timestep ts, Renderer& renderer)
	{
        if (_Entity != ENTITY_INVALID)
        {
            /* calculate the different between the camera viewport and the sceneviewer to set the scalling */
            float diff = _CameraController->GetSize() / std::min<float>(_SceneViewer->GetViewport()._size.x, _SceneViewer->GetViewport()._size.y);

            //Transform2DComponent* transform2DComponent = _SceneViewer->GetScene()->Get<Transform2DComponent>(_Entity);
            //
            //Wyrd::DrawVertex2DCommand cmd{};
            //cmd.type = 1;
            //cmd.position = transform2DComponent->position;
            //cmd.vertices = &_Vertices;
            //cmd.vpMatrix = _CameraController->GetCamera().GetViewProjectionMatrix();
            //cmd.shader = Application::Get().GetResources().Shaders["Vertex2D"].get();
            //cmd.color = { 1.0f, 1.0f, 0.0f, 1.0f };
            //
            //renderer.Submit(cmd);
            //
            //renderer.Flush();
        }
	}

    void TranslationGizmo::OnEvent(Event& event)
    {
        if (_Entity != ENTITY_INVALID)
        {
            EventDispatcher dispatcher(event);
            dispatcher.Dispatch<MouseMovedEvent>(WYRD_BIND_EVENT_FN(TranslationGizmo::OnMouseMovedEvent), nullptr);
            dispatcher.Dispatch<MouseButtonPressedEvent>(WYRD_BIND_EVENT_FN(TranslationGizmo::OnMouseButtonPressedEvent), nullptr);
            dispatcher.Dispatch<MouseButtonReleasedEvent>(WYRD_BIND_EVENT_FN(TranslationGizmo::OnMouseButtonReleasedEvent), nullptr);
        }
    }

    bool TranslationGizmo::OnMouseMovedEvent(MouseMovedEvent& e, void* data)
    {
        float diff = _CameraController->GetSize() / std::min<float>(_SceneViewer->GetViewport()._size.x, _SceneViewer->GetViewport()._size.y);

        //Transform2DComponent* transform2DComponent = _SceneViewer->GetScene()->Get<Transform2DComponent>(_Entity);
        //
        //glm::vec4 worldSpace = glm::vec4(_SceneViewer->Convert2DToWorldSpace({ e.GetX(), e.GetY() }), -1.0f, 1.0f);
        //
        //glm::vec4 modelSpace = glm::inverse(transform2DComponent->CalculateModelMatrix() * glm::scale(glm::vec3(diff, diff, diff))) * worldSpace;
        //
        //Point ptCoords = { modelSpace.x, modelSpace.y };
        //
        //bool updateVertices = false;
        //
        //if (_InputState == InputState::NONE || _InputState == InputState::OVER)
        //{
        //    /* test each of the handles to see if we have a mouse event */
        //    if (InsidePolygon(_Vertices, _VertexGroups["XY_Handle"], ptCoords))
        //    {
        //        SetAxisState(AxisState::XY);
        //        SetInputState(InputState::OVER);
        //    } 
        //    else if (InsidePolygon(_Vertices, _VertexGroups["X_Handle"], ptCoords))
        //    {
        //        SetAxisState(AxisState::X);
        //        SetInputState(InputState::OVER);
        //    } 
        //    else if (InsidePolygon(_Vertices, _VertexGroups["Y_Handle"], ptCoords))
        //    {
        //        SetAxisState(AxisState::Y);
        //        SetInputState(InputState::OVER);
        //    }
        //    else
        //    {
        //        SetAxisState(AxisState::NONE);
        //        SetInputState(InputState::NONE);
        //    }
        //}else
        //{
        //    glm::vec2 worldSpaceDelta = _LastMouseWorldPos - _SceneViewer->GetMouseWorldPos();
        //    
        //    switch (_AxisState)
        //    {
        //    case AxisState::XY:
        //        transform2DComponent->position.x += -worldSpaceDelta.x;
        //        transform2DComponent->position.y += -worldSpaceDelta.y;
        //        break;
        //    case AxisState::X:
        //        transform2DComponent->position.x += -worldSpaceDelta.x;
        //        break;
        //    case AxisState::Y:
        //        transform2DComponent->position.y += -worldSpaceDelta.y;
        //        break;
        //    }
        //    
        //    _LastMouseWorldPos = _SceneViewer->GetMouseWorldPos();
        //}
                
        return true;
    }


    bool TranslationGizmo::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e, void* data)
    {
        _SceneViewer->Convert2DToWorldSpace({ e.GetPositionX(), e.GetPositionY() });

        if (_InputState == InputState::OVER)
        {
            SetInputState(InputState::ACTIVE);
        }

        glm::vec2 viewportMouseCoords = _SceneViewer->GetViewportSpaceFromPoint({ e.GetPositionX(), e.GetPositionY() });;

        _LastMouseWorldPos = _CameraController->GetCamera().GetEyeSpaceFromPoint(viewportMouseCoords, _SceneViewer->GetViewport());

        return true;
    }

    bool TranslationGizmo::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e, void* data)
    {
        SetInputState(InputState::NONE);
        return true;
    }

	void TranslationGizmo::OnSelectedEntityChanged(Events::EventArgs& args)
	{
		Events::SelectedEntityChangedArgs& evtArgs = static_cast<Events::SelectedEntityChangedArgs&>(args);

        _Entity = evtArgs.entity;
	}


    void TranslationGizmo::SetInputState(InputState state)
    {
        if (state != _InputState)
        {
            _InputState = state;
        }
    }


    void TranslationGizmo::SetAxisState(AxisState state)
    {
        if (state != _AxisState)
        {
            switch (state)
            {
            case AxisState::NONE:
                UpdateVertexGroupColor("XY_Handle", colorXYAxisNormal);
                UpdateVertexGroupColor("X_Handle", colorXAxisNormal);
                UpdateVertexGroupColor("Y_Handle", colorYAxisNormal);
                break;
            case AxisState::X:
                UpdateVertexGroupColor("XY_Handle", colorXYAxisNormal);
                UpdateVertexGroupColor("X_Handle", colorXAxisOver);
                UpdateVertexGroupColor("Y_Handle", colorYAxisNormal);
                break;
            case AxisState::Y:
                UpdateVertexGroupColor("XY_Handle", colorXYAxisNormal);
                UpdateVertexGroupColor("X_Handle", colorXAxisNormal);
                UpdateVertexGroupColor("Y_Handle", colorYAxisOver);
                break;
            case AxisState::XY:
                UpdateVertexGroupColor("XY_Handle", colorXYAxisOver);
                UpdateVertexGroupColor("X_Handle", colorXAxisNormal);
                UpdateVertexGroupColor("Y_Handle", colorYAxisNormal);
                break;
            }

            //_VertexBuffer->Update((float*)&_Vertices[0], _Vertices.size() * sizeof(Vertex2D), 0);

            _AxisState = state;
        }
    }

    void TranslationGizmo::SetMovementState(MovementState state)
    {

    }

	void TranslationGizmo::UpdateVertexGroupColor(const std::string& groupName, const Color& color)
	{
		for (int i = _VertexGroups[groupName].first; i <= (int)_VertexGroups[groupName].second; i++)
		{
			_Vertices[i].r = color.r;
			_Vertices[i].g = color.g;
			_Vertices[i].b = color.b;
			_Vertices[i].a = color.a;
		}
	}
}