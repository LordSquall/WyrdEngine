#pragma once

#include <core/Layer.h>
#include <graphics/Mesh.h>

class TestRenderLayer : public Osiris::Layer
{
public:
	TestRenderLayer();

	void OnRender(std::shared_ptr<Osiris::Renderer> renderer) override;
private:
	std::vector<Osiris::Mesh> m_Meshs;
};