#pragma once

#include "ExampleLayer.h"

bool ExampleLayer::OnAttach()
{
	return true;
}

void ExampleLayer::OnDetach()
{

}

void ExampleLayer::OnUpdate(Timestep ts)
{

}

void ExampleLayer::OnRender(Timestep ts, Renderer& renderer)
{
	renderer.DrawRectangle(0, 0, 50, 50);
}