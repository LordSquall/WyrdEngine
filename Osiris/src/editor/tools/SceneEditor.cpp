#pragma once

#include "osrpch.h"

#include "SceneEditor.h"
#include "editor/scene/Node.h"

#include "core/Application.h"
#include "core/Layer.h"

#include "events/Event.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

#include "imgui.h"

namespace Osiris::Editor
{

	SceneEditor::SceneEditor() : EditorPlugin("Scene Editor")
	{
		m_Scene = Scene();

		/* Build fake tree */
		Node& rootNode = m_Scene.GetRootNode();

		rootNode.AddChild(new Node(std::string("Node 1")));

		Node* subNode2 = new Node(std::string("Node 2"));
		subNode2->AddChild(new Node(std::string("Node 2-1")));
		subNode2->AddChild(new Node(std::string("Node 2-2")));
		subNode2->AddChild(new Node(std::string("Node 2-3")));

		rootNode.AddChild(subNode2);

		rootNode.AddChild(new Node(std::string("Node 3")));
		rootNode.AddChild(new Node(std::string("Node 4")));
		rootNode.AddChild(new Node(std::string("Node 5")));
	}

	SceneEditor::~SceneEditor(){}

	void SceneEditor::OnEditorRender()
	{
		ImGui::Begin("Scene Editor");

		PopulateSceneTree(&m_Scene.GetRootNode());

		ImGui::End();
	}

	void SceneEditor::PopulateSceneTree(Node* rootNode)
	{
		for (Node* node : *rootNode)
		{
			if (ImGui::TreeNode(node->GetName().c_str()))
			{
				PopulateSceneTree(node);
				ImGui::TreePop();
			}
		}
	}

}