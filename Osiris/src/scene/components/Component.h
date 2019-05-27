#pragma once

#include "osrpch.h"

#include "scene/Visitable.h"

namespace Osiris {

	class OSIRIS_API Component : Visitable
	{
	public:
		Component();
		~Component();

		// Inherited via Visitable
		virtual void AcceptInOrder(GameObjectVisitor & visitor) = 0;

		virtual void AcceptPreOrder(GameObjectVisitor & visitor) = 0;

		virtual void AcceptPostOrder(GameObjectVisitor & visitor) = 0;

	};
}