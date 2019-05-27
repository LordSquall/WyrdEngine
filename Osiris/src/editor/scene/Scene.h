#pragma once

#include "Node.h"

namespace Osiris::Editor
{
	class Scene
	{
	public:
		Scene()
		{
			m_Root = Node(std::string("SceneRoot"));
		}

		virtual ~Scene() { }


		inline Node& GetRootNode() { return m_Root; }

	private:
		Node m_Root;
	};
}