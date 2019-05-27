#pragma once

#include "osrpch.h"

#include "GameObjectVisitor.h"

namespace Osiris {

	class OSIRIS_API Visitable
	{
	public:
		void virtual AcceptInOrder(GameObjectVisitor & visitor) = 0;
		void virtual AcceptPreOrder(GameObjectVisitor & visitor) = 0;
		void virtual AcceptPostOrder(GameObjectVisitor & visitor) = 0;
	};
}