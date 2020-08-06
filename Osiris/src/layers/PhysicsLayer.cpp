#pragma once

#include "osrpch.h"

#include "PhysicsLayer.h"

namespace Osiris::Layers
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