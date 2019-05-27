#pragma once

#include <vector>

namespace Osiris::Editor
{
	class Node
	{
	public:
		Node(std::string& name = std::string("Node"))
		{
			m_Name = name;
		}

		virtual ~Node() {}


		inline std::string& GetName() { return m_Name; }
		inline void SetName(std::string name) { m_Name = name; }



		void AddChild(Node* node);



		std::vector<Node*>::iterator begin() { return m_Children.begin(); }
		std::vector<Node*>::iterator end() { return m_Children.end(); }

	private:
		std::string m_Name;
		std::vector<Node*> m_Children;
	};
}