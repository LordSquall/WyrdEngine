#pragma once

#include "osrpch.h"


namespace Osiris {

	class GameObject;
	class MeshRenderer;

	class OSIRIS_API GameObjectVisitor
	{
	public:
		void virtual Visit(GameObject* gameObject) = 0;
		void virtual Visit(MeshRenderer* meshRenderer) = 0;
	};
}