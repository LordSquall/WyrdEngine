/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/export.h>
#include <core/Timestep.h>
#include <core/Application.h>
#include <core/support/PrimitiveUtils.h>

/* local includes */
#include "Translate2DGizmo.h"
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
    Translate2DGizmo::Translate2DGizmo(SceneViewer* sceneViewer) : Gizmo(sceneViewer, ENTITY_INVALID), _DragActive(false), _ActiveRegion(-1)
	{
		/* retrieve the services */
		_EventService = ServiceManager::Get<EventService>();

		/* setup event subscriptions */
		_EventService->Subscribe(Events::EventType::SelectedEntityChanged, WYRD_BIND_FN(Translate2DGizmo::OnSelectedEntityChanged));
	}

    Translate2DGizmo::~Translate2DGizmo() {}

	void Translate2DGizmo::Render(Timestep ts, Renderer& renderer)
	{
       //if (_Entity != ENTITY_INVALID)
       //{
       //    /* calculate the different between the camera viewport and the sceneviewer to set the scalling */
       //    float diff = _CameraController->GetSize() / std::min<float>(_SceneViewer->GetViewport()._size.x, _SceneViewer->GetViewport()._size.y);
       //
       //    Transform2DComponent* transform2DComponent = _SceneViewer->GetScene()->Get<Transform2DComponent>(_Entity);
       //    
       //    Wyrd::DrawVertex2DCommand cmd{};
       //    cmd.type = 1;
       //    cmd.position = transform2DComponent->position;
       //    cmd.vertices = &_Vertices;
       //    cmd.vpMatrix = _CameraController->GetCamera().GetViewProjectionMatrix();
       //    cmd.shader = Application::Get().GetResources().Shaders["Vertex2D"].get();
       //    cmd.color = { 1.0f, 1.0f, 1.0f, 1.0f };
       //    cmd.drawType = RendererDrawType::Triangles;
       //    
       //    //Build();
       //    
       //    renderer.Submit(cmd);
       //}
	}
    
    void Translate2DGizmo::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseMovedEvent>(WYRD_BIND_EVENT_FN(Translate2DGizmo::OnMouseMovedEvent), nullptr);
        dispatcher.Dispatch<MouseButtonPressedEvent>(WYRD_BIND_EVENT_FN(Translate2DGizmo::OnMouseButtonPressedEvent), nullptr);
    }

	void Translate2DGizmo::OnSelectedEntityChanged(Events::EventArgs& args)
	{
		Events::SelectedEntityChangedArgs& evtArgs = static_cast<Events::SelectedEntityChangedArgs&>(args);

        _Entity = evtArgs.entity;

        Build();
	}

    bool Translate2DGizmo::OnMouseMovedEvent(MouseMovedEvent& e, void* data)
    {
        Transform2DComponent* transform2DComponent = _SceneViewer->GetScene()->Get<Transform2DComponent>(_Entity);
        if (transform2DComponent != nullptr)
        {
            glm::vec2 worldPos = _SceneViewer->Convert2DToWorldSpace({ e.GetX(), e.GetY() });
            glm::vec2 mousePos = { worldPos.x + -transform2DComponent->position.x, worldPos.y + -transform2DComponent->position.y };

            // check if we are actively performing a drag operation
            if (_DragActive && _ActiveRegion != -1)
            {
                // calculate mouse delta
                Vector2 delta = _StartDragPosition - Vector2 { mousePos.x, mousePos.y };
                if (_Regions[_ActiveRegion].onDragFunc != nullptr)
                {
                    _Regions[_ActiveRegion].onDragFunc(delta);
                }
            }
            else
            {
                // check if we have hit a new region
                int newRegion = _ActiveRegion;
                bool regionFound = false;
                for (int i = 0; i < _Regions.size(); i++)
                {
                    if (_Regions[i].area.ContainsPoint(glm::vec4(mousePos, -1.0f, 1.0f)))
                    {
                        newRegion = i;
                        regionFound = true;
                    }
                }

                if (regionFound == false && !_DragActive)
                {
                    if (_ActiveRegion != -1)
                    {
                        if (_Regions[_ActiveRegion].mouseExitFunc != nullptr)
                        {
                            _Regions[_ActiveRegion].mouseExitFunc(_ActiveRegion);
                        }
                    }
                    _ActiveRegion = -1;
                    return false;
                }

                if (_ActiveRegion != newRegion)
                {
                    if (_ActiveRegion != -1)
                    {
                        if (_Regions[_ActiveRegion].mouseExitFunc != nullptr)
                        {
                            _Regions[_ActiveRegion].mouseExitFunc(_ActiveRegion);
                        }
                    }

                    // update the look and feel based on current region
                    for (int i = 0; i < _Regions.size(); i++)
                    {
                        if (newRegion == i)
                        {
                            if (_Regions[i].mouseEnterFunc != nullptr)
                            {
                                _Regions[i].mouseEnterFunc(i);
                            }
                        }
                    }

                    _ActiveRegion = newRegion;
                }
            }
        }

        return false;
    }

    bool Translate2DGizmo::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e, void* data)
    {
        if (_ActiveRegion != -1)
        {
            glm::vec2 worldPos = _SceneViewer->Convert2DToWorldSpace({ e.GetPositionX(), e.GetPositionY() });
            _StartDragPosition = { worldPos.x , worldPos.y };

            _DragActive = true;
        }
        return false;
    }

    bool Translate2DGizmo::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e, void* data)
    {
        _DragActive = false;

        return false;
    }

    void Translate2DGizmo::Build()
    {
        EditorComponent* editorComponent = _SceneViewer->GetScene()->Get<EditorComponent>(_Entity);
        Transform2DComponent* transform2DComponent = _SceneViewer->GetScene()->Get<Transform2DComponent>(_Entity);
        if (editorComponent != nullptr && transform2DComponent != nullptr)
        {
            /* create the current set of vertices and regions */
            _Vertices.clear();
            _Regions.clear();
            _ActiveRegion = -1;

            /* setup regions */
            Region yAxixHandleRegion;
            yAxixHandleRegion.area = { { 10.0f, -2.0f }, { 20.0f, 4.0f } };
            yAxixHandleRegion.color = Color::RED;
            yAxixHandleRegion.hoverColor = Color::MAGENTA;
            yAxixHandleRegion.mouseEnterFunc = [this] (int offset) {
                PrimitiveUtils::UpdateColor(_Vertices, _Regions[offset].vertexOffset, PrimitiveUtils::RectCount, Color::MAGENTA);
            };

            yAxixHandleRegion.mouseExitFunc = [this](int offset) {
                PrimitiveUtils::UpdateColor(_Vertices, _Regions[offset].vertexOffset, PrimitiveUtils::RectCount, Color::RED);
            };

            //yAxixHandleRegion.onDragFunc = [transform2DComponent](Vector2 delta) {
            //    transform2DComponent->position.y += delta.y;
            //};

            Region xAxixHandleRegion;
            xAxixHandleRegion.area = { { -2.0f, 10.0f }, { 4.0f, 20.0f } };
            xAxixHandleRegion.color = Color::GREEN;
            xAxixHandleRegion.hoverColor = Color::MAGENTA;
            xAxixHandleRegion.mouseEnterFunc = [this](int offset) {
                PrimitiveUtils::UpdateColor(_Vertices, _Regions[offset].vertexOffset, PrimitiveUtils::RectCount, Color::MAGENTA);
            };

            xAxixHandleRegion.mouseExitFunc = [this](int offset) {
                PrimitiveUtils::UpdateColor(_Vertices, _Regions[offset].vertexOffset, PrimitiveUtils::RectCount, Color::GREEN);
            };

            //xAxixHandleRegion.onDragFunc = [transform2DComponent](Vector2 delta) {
            //    transform2DComponent->position.x += delta.x;
            //};

            Region xyAxisHandleRegion;
            xyAxisHandleRegion.area = { { -5.0f, -5.0f }, { 10.0f, 10.0f } };
            xyAxisHandleRegion.color = Color::WHITE;
            xyAxisHandleRegion.hoverColor = Color::MAGENTA;
            xyAxisHandleRegion.mouseEnterFunc = [this](int offset) {
                PrimitiveUtils::UpdateColor(_Vertices, _Regions[offset].vertexOffset, PrimitiveUtils::RectCount, Color::MAGENTA);
            };

            xyAxisHandleRegion.mouseExitFunc = [this](int offset) {
                PrimitiveUtils::UpdateColor(_Vertices, _Regions[offset].vertexOffset, PrimitiveUtils::RectCount, Color::WHITE);
            };

            //xyAxisHandleRegion.onDragFunc = [transform2DComponent](Vector2 delta) {
            //    transform2DComponent->position.x = delta.x;
            //    transform2DComponent->position.y = delta.y;
            //};



            uint32_t offset = 0;

            /* entity outline */
            //offset += PrimitiveUtils::GenerateRect(_Vertices, offset, { 0.0f, 0.0f }, editorComponent->inputArea._size, Color::CYAN, 2.0f, transform2DComponent->rotation, { 0.0f, 0.0f });
            //
            ///* axis movement*/
            //yAxixHandleRegion.vertexOffset = offset;
            //offset += PrimitiveUtils::GenerateRect(_Vertices, offset, yAxixHandleRegion.area._position, yAxixHandleRegion.area._size, yAxixHandleRegion.color, 2.0f, transform2DComponent->rotation, { 0.0f, 0.0f });
            //
            //xAxixHandleRegion.vertexOffset = offset;
            //offset += PrimitiveUtils::GenerateRect(_Vertices, offset, xAxixHandleRegion.area._position, xAxixHandleRegion.area._size, xAxixHandleRegion.color, 2.0f, transform2DComponent->rotation, { 0.0f, 0.0f });
            //
            ///* origin */
            //xyAxisHandleRegion.vertexOffset = offset;
            //offset += PrimitiveUtils::GenerateRect(_Vertices, offset, xyAxisHandleRegion.area._position, xyAxisHandleRegion.area._size, xyAxisHandleRegion.color, 2.0f, transform2DComponent->rotation, { 0.0f, 0.0f });
            
            _Regions.push_back(yAxixHandleRegion);
            _Regions.push_back(xAxixHandleRegion);
            _Regions.push_back(xyAxisHandleRegion);
        }
    }
}