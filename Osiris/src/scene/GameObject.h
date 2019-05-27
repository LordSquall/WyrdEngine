#pragma once

#include "osrpch.h"

#include "Visitable.h"
#include "components/Component.h"

namespace Osiris {

	class OSIRIS_API GameObject : public Visitable
	{
	public:
		GameObject();
		~GameObject();

		void AddComponent(Component* component);

		void Shutdown();

		// Inherited via Visitable
		virtual void AcceptInOrder(GameObjectVisitor & visitor) override;
		virtual void AcceptPreOrder(GameObjectVisitor & visitor) override;
		virtual void AcceptPostOrder(GameObjectVisitor & visitor) override;

	protected:
		std::vector<GameObject*> m_Children;
		std::vector<Component*> m_Components;
	};
}