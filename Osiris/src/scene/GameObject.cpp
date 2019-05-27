#include "osrpch.h"
#include "GameObject.h"

namespace Osiris {

	GameObject::GameObject()
	{
		
	}

	GameObject::~GameObject()
	{

	}

	void GameObject::AddComponent(Component* component)
	{
		m_Components.emplace_back(component);
	}

	void GameObject::Shutdown()
	{

	}

	void GameObject::AcceptInOrder(GameObjectVisitor &visitor)
	{
		visitor.Visit(this);

		for (auto it = m_Components.end(); it != m_Components.begin(); )
		{
			(*--it)->AcceptPostOrder(visitor);
		}
	}

	void GameObject::AcceptPreOrder(GameObjectVisitor & visitor)
	{
		visitor.Visit(this);

		for (auto it = m_Components.end(); it != m_Components.begin(); )
		{
			(*--it)->AcceptPostOrder(visitor);
		}
	}

	void GameObject::AcceptPostOrder(GameObjectVisitor & visitor)
	{
		for (auto it = m_Components.end(); it != m_Components.begin(); )
		{
			(*--it)->AcceptPostOrder(visitor);
		}

		visitor.Visit(this);
	}
}