#pragma once

#include <Osiris.h>

#include <scene/GameObject.h>
#include <scene/visitors/RenderVisitor.h>
#include <scene/components/MeshRenderer.h>

#ifdef OSR_EDITOR_ENABLED
#include "editor/layers/EditorLayer.h"
#endif

using namespace Osiris;

class ExampleLayer : public Layer
{
public:
	ExampleLayer() : Layer("Example Layer")
	{

	}

	void OnAttach() override
	{
		root = new GameObject();

		root->AddComponent(new MeshRenderer());
	}

	void OnDetach() override
	{
		root->Shutdown();
	}

	void OnUpdate() override
	{
		
	}

	void OnEvent(Event& event) override
	{

	}

	void OnRender(std::shared_ptr<Renderer> renderer) override
	{
		root->AcceptPostOrder(visitor);
	}

private:
	GameObject* root;
	RenderVisitor visitor;
};