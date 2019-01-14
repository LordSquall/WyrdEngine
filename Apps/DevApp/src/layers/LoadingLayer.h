#pragma once

#include <core/Layer.h>


class LoadingLayer : public Osiris::Layer
{
public:
	LoadingLayer();

	void OnUpdate() override;

	void OnEvent(Osiris::Event& event) override;
	
};