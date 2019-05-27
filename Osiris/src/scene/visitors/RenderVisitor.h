#pragma once

#include "osrpch.h"

#include "scene/GameObjectVisitor.h"

namespace Osiris {

	class OSIRIS_API RenderVisitor : public GameObjectVisitor
	{
	public:
		RenderVisitor();
		~RenderVisitor();


		// Inherited via GameObjectVisitor
		virtual void Visit(GameObject * gameObject) override;

		virtual void Visit(MeshRenderer * meshRenderer) override;

	};
}