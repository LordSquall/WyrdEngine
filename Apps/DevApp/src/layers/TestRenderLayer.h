#pragma once

#include <core/Layer.h>
#include <graphics/Mesh.h>

#include "../Resources.h"

class TestRenderLayer : public Osiris::Layer
{
public:
	TestRenderLayer(std::shared_ptr<Resources> resources);

	void OnRender(std::shared_ptr<Osiris::Renderer> renderer) override;
private:
	std::shared_ptr<Resources> m_Resources;

	std::vector<Osiris::Mesh> m_Meshs;
};