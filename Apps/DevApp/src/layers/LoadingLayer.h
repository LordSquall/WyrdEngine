#pragma once

#include <core/Layer.h>

#include "../Resources.h"

class LoadingLayer : public Osiris::Layer
{
public:
	LoadingLayer(std::shared_ptr<Resources> resources);

	void OnUpdate() override;

	void OnEvent(Osiris::Event& event) override;
	
private:
	std::shared_ptr<Resources> m_Resources;
};