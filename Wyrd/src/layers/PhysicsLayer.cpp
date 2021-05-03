#pragma once

#include "wyrdpch.h"

#include "layers/PhysicsLayer.h"
#include "core/Application.h"

namespace Wyrd
{
	PhysicsLayer::PhysicsLayer() : Layer("Physics_Layer")
	{

	}

	bool PhysicsLayer::OnAttach()
	{
		return true;
	}

	void PhysicsLayer::OnDetach()
	{
	}

	void PhysicsLayer::OnUpdate(Timestep ts)
	{
		Application::Get().GetPhysics().Update(ts);
	}

	void PhysicsLayer::OnRender(Timestep ts, Renderer& renderer)
	{
	}
}