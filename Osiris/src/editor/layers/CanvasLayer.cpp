#pragma once

#include "osrpch.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "CanvasLayer.h"

#include "core/Application.h"
#include "core/Layer.h"


namespace Osiris::Editor
{
	CanvasLayer::CanvasLayer()
		: Layer("CanvasLayer")
	{

		m_XPosition = 1280.0f / 2.0f;
		m_YPosition = 720.0f / 2.0f;
		m_ZPosition = 0.0f;
	}

	CanvasLayer::~CanvasLayer()
	{

	}

	void CanvasLayer::OnAttach()
	{
		float vertices[] = {
			50.0f,  50.0f, 1.0f,  1.0f, 0.0f,	// top right
			50.0f, -50.0f, 1.0f,  1.0f, 1.0f,	// bottom right
		   -50.0f, -50.0f, 1.0f,  0.0f, 1.0f,	// bottom left
		   -50.0f,  50.0f, 1.0f,  0.0f, 0.0f	// top left 
		};

		
		unsigned int indices[] = {  // note that we start from 0!
			0, 1, 3,  // first Triangle
			1, 2, 3   // second Triangle
		};

		unsigned int VBO, EBO;

		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
		// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
		glBindVertexArray(0);
	}

	void CanvasLayer::OnDetach()
	{

	}

	void CanvasLayer::OnRender(std::shared_ptr<Renderer> renderer)
	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(m_XPosition, m_YPosition, m_ZPosition));
		glm::mat4 view = m_EditorCamera.GetViewMatrix();
		glm::mat4 projection = m_EditorCamera.GetProjectionMatrix();

		int modelLoc = glGetUniformLocation(1, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		int viewLoc = glGetUniformLocation(1, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		int projectionLoc = glGetUniformLocation(1, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindTexture(GL_TEXTURE_2D, 1);

		glBindVertexArray(m_VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	   //glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void CanvasLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(OSR_BIND_EVENT_FN(CanvasLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(OSR_BIND_EVENT_FN(CanvasLayer::OnMouseScrollEvent));
	}


	bool CanvasLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		switch(e.GetKeyCode())
		{
		case GLFW_KEY_UP:
			m_YPosition += 1.0f;
			break;
		case GLFW_KEY_DOWN:
			m_YPosition -= 1.0f;
			break;
		case GLFW_KEY_LEFT:
			m_XPosition -= 1.0f;
			break;
		case GLFW_KEY_RIGHT:
			m_XPosition += 1.0f;
			break;
		}
		return true;
	}

	bool CanvasLayer::OnMouseScrollEvent(MouseScrolledEvent& e)
	{
		
		return false;
	}
}