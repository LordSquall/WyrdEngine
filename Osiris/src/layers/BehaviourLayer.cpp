#pragma once

#include "osrpch.h"

#include "BehaviourLayer.h"

namespace Osiris::Layers
{
	BehaviourLayer::BehaviourLayer() : Layer("Behaviour_Layer")
	{

	}

	bool BehaviourLayer::OnAttach()
	{
		return true;
	}

	void BehaviourLayer::OnDetach()
	{
	}

	void BehaviourLayer::OnUpdate(Timestep ts)
	{
		Application::Get().GetBehaviour().Update(ts);
	}

	void BehaviourLayer::OnRender(Timestep ts, Renderer& renderer)
	{
	}
}