#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/Log.h>
#include <core/export.h>
#include <core/Timestep.h>
#include <core/Application.h>
#include <core/scene/components/Transform2DComponent.h>

/* local includes */
#include "TranslationGizmo.h"
#include "views/SceneViewer/SceneViewer.h"
#include "services/ServiceManager.h"
#include "services/EventsService.h"
#include "events/EditorEvents.h"
#include "loaders/ObjLoader.h"

/* external includes */
#include <glm/glm.hpp>

namespace Osiris::Editor
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

    bool InsidePolygon(std::vector<GizmoVertex>& vertices, std::pair<uint32_t, uint32_t>& range, Point p)//Point* polygon, int N, Point p)
    {
        float precision = 0.00001f;
        int counter = 0;
        int i;
        float xinters;
        GizmoVertex p1, p2;
        int n = range.second - range.first;

        p1 = vertices[range.first];
        for (i = 0; i <= n; i++) {

            uint32_t next = range.first + (i + 1);
            if (next > range.second)
                next = range.first;

            p2 = vertices[next];
            if (p.y > std::min(p1.y, p2.y)) {
                if (p.y <= std::max(p1.y, p2.y)) {
                    if (p.x <= std::max(p1.x, p2.x)) {
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

    TranslationGizmo::TranslationGizmo(SceneViewer* sceneViewer) : Gizmo(sceneViewer), _InputState(InputState::NONE), _AxisState(AxisState::XY), _MovementState(MovementState::LOCAL), _LastMouseWorldPos(0.0f, 0.0f)
	{
		/* retrieve the services */
		_EventService = ServiceManager::Get<EventService>(ServiceManager::Events);

		/* setup event subscriptions */
		_EventService->Subscribe(Events::EventType::SelectedGameObjectChanged, EVENT_FUNC(TranslationGizmo::OnSelectedGameObjectChanged));
		
		/* retrieve the shader */
		_Shader = Application::Get().GetResources().Shaders["Gizmo"];

		/* create and bind a default VAO */
		_VertexArray.reset(VertexArray::Create());
		_VertexArray->Bind();

        /* load the vertices from an external source file */
		ObjLoader::Load(Utils::GetEditorResFolder() + "gizmos\\translation_gizmo.obj", _Vertices, _Indices, _VertexGroups, 0.4f);

		/* update handle colors */
		UpdateVertexGroupColor("XY_Handle", colorXYAxisNormal);
		UpdateVertexGroupColor("Y_Handle", colorYAxisNormal);
		UpdateVertexGroupColor("X_Handle", colorXAxisNormal);

		/* create a new vertex and Index buffer on the GPU */
		_VertexBuffer.reset(VertexBuffer::Create((float*)&_Vertices[0], _Vertices.size() * sizeof(GizmoVertex), "Translation VBO"));
		_IndexBuffer.reset(IndexBuffer::Create(&_Indices[0], _Indices.size()));

		/* setup the vertex array attribute data */
		_VertexArray->SetAttribute(0, 0, 2, sizeof(GizmoVertex));
		_VertexArray->SetAttribute(1, 2, 4, sizeof(GizmoVertex));

		/* bind the shader */
		_Shader->Bind();

		/* set the vp matrix to a standard otho matrix */
		_Shader->SetVPMatrix(_CameraController->GetCamera().GetViewProjectionMatrix());

	}

	TranslationGizmo::~TranslationGizmo() {}


	void TranslationGizmo::Render(Timestep ts, Renderer& renderer)
	{
		/* calculate the different between the camera viewport and the sceneviewer to set the scalling */
        float diff = _CameraController->GetSize() / std::min(_SceneViewer->GetViewport().size.x, _SceneViewer->GetViewport().size.y);

        /* setup the pipeline objects */
        _VertexArray->Bind();
        _Shader->Bind();
        _VertexBuffer->Bind();
        _IndexBuffer->Bind();

        /* render using the cameras VP matrix */
        _Shader->SetVPMatrix(_CameraController->GetCamera().GetViewProjectionMatrix());

        /* set the scale matrix for the gizmo */
        _Shader->SetMatrix("scale", glm::scale(glm::vec3(diff, diff, diff)));
                
        /* Set the model matrix of the gameobject, however we want to ammend the scale by the viewport difference */
        _Shader->SetMatrix("model", _GameObject->transform2D->matrix);

        /* set the default uniforms */
        _Shader->SetUniformVec4("blendColor", glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });

        /* perform the draw */
		renderer.DrawElements(RendererDrawType::Triangles, _IndexBuffer->GetCount());
	}


    void TranslationGizmo::OnEvent(Event& event)
    {
        if (_GameObject != nullptr)
        {
            EventDispatcher dispatcher(event);
            dispatcher.Dispatch<MouseMovedEvent>(OSR_BIND_EVENT_FN(TranslationGizmo::OnMouseMovedEvent));
            dispatcher.Dispatch<MouseButtonPressedEvent>(OSR_BIND_EVENT_FN(TranslationGizmo::OnMouseButtonPressedEvent));
            dispatcher.Dispatch<MouseButtonReleasedEvent>(OSR_BIND_EVENT_FN(TranslationGizmo::OnMouseButtonReleasedEvent));
        }
    }

    bool TranslationGizmo::OnMouseMovedEvent(MouseMovedEvent& e)
    {
        float diff = _CameraController->GetSize() / std::min(_SceneViewer->GetViewport().size.x, _SceneViewer->GetViewport().size.y);

        vec4 worldSpace = glm::vec4(_SceneViewer->Convert2DToWorldSpace({ e.GetX(), e.GetY() }), -1.0f, 1.0f);

        vec4 modelSpace = glm::inverse(_GameObject->transform2D->matrix * glm::scale(glm::vec3(diff, diff, diff))) * worldSpace;

        Point ptCoords = { modelSpace.x, modelSpace.y };

        bool updateVertices = false;

        if (_InputState == InputState::NONE || _InputState == InputState::OVER)
        {
            /* test each of the handles to see if we have a mouse event */
            if (InsidePolygon(_Vertices, _VertexGroups["XY_Handle"], ptCoords))
            {
                SetAxisState(AxisState::XY);
                SetInputState(InputState::OVER);
            } 
            else if (InsidePolygon(_Vertices, _VertexGroups["X_Handle"], ptCoords))
            {
                SetAxisState(AxisState::X);
                SetInputState(InputState::OVER);
            } 
            else if (InsidePolygon(_Vertices, _VertexGroups["Y_Handle"], ptCoords))
            {
                SetAxisState(AxisState::Y);
                SetInputState(InputState::OVER);
            }
            else
            {
                SetAxisState(AxisState::NONE);
                SetInputState(InputState::NONE);
            }
        }else
        {
            glm::vec2 worldSpaceDelta = _LastMouseWorldPos - _SceneViewer->GetMouseWorldPos();

            switch (_AxisState)
            {
            case AxisState::XY:
                _GameObject->transform2D->Translate({ -worldSpaceDelta.x, -worldSpaceDelta.y });
                break;
            case AxisState::X:
                _GameObject->transform2D->Translate({ -worldSpaceDelta.x, 0.0f });
                break;
            case AxisState::Y:
                _GameObject->transform2D->Translate({ 0.0f, -worldSpaceDelta.y });
                break;
            }

            _LastMouseWorldPos = _SceneViewer->GetMouseWorldPos();
        }
                
        return true;
    }


    bool TranslationGizmo::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
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

    bool TranslationGizmo::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
    {
        SetInputState(InputState::NONE);
        return true;
    }

	void TranslationGizmo::OnSelectedGameObjectChanged(Events::EventArgs& args)
	{
		Events::SelectedGameObjectChangedArgs& evtArgs = static_cast<Events::SelectedGameObjectChangedArgs&>(args);

		_GameObject = evtArgs.gameObject;
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

            _VertexBuffer->Update((float*)&_Vertices[0], _Vertices.size() * sizeof(GizmoVertex), 0);

            _AxisState = state;
        }
    }

    void TranslationGizmo::SetMovementState(MovementState state)
    {

    }

	void TranslationGizmo::UpdateVertexGroupColor(const std::string& groupName, const Color& color)
	{
		for (int i = _VertexGroups[groupName].first; i <= _VertexGroups[groupName].second; i++)
		{
			_Vertices[i].r = color.r;
			_Vertices[i].g = color.g;
			_Vertices[i].b = color.b;
			_Vertices[i].a = color.a;
		}
	}
}