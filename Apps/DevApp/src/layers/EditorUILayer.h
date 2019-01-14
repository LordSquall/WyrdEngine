#pragma once

#include <core/Layer.h>
#include <graphics/Mesh.h>

class EditorUILayer : public Osiris::Layer
{
public:
	EditorUILayer();

	void OnRender(std::shared_ptr<Osiris::Renderer> renderer) override;
};