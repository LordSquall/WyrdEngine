#pragma once

#include "osrpch.h"

#include "Node.h"

#include "core/Application.h"


namespace Osiris::Editor
{
	void Node::AddChild(Node* node)
	{ 
		m_Children.push_back(node);
	}
}