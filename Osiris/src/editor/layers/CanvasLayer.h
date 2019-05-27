#pragma once

#include "core/export.h"
#include "core/Layer.h"

#include "graphics/Camera.h"

#include "events/KeyEvent.h"
#include "events/MouseEvent.h"
#include "events/ApplicationEvent.h"

namespace Osiris::Editor
{
	class CanvasLayer : public Layer
	{
	public:
		CanvasLayer();
		~CanvasLayer();

		
		void OnAttach() override;
		void OnDetach() override;
		void OnRender(std::shared_ptr<Renderer> renderer) override;
		void OnEvent(Event& event) override;

	private:
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnMouseScrollEvent(MouseScrolledEvent& e);

	private:
		Osiris::Camera m_EditorCamera;
		unsigned int m_VAO;

		float m_XPosition;
		float m_YPosition;
		float m_ZPosition;

	};
}