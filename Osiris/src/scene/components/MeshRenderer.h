#pragma once

#include "osrpch.h"

#include "Component.h"


namespace Osiris {

	class OSIRIS_API MeshRenderer : public Component
	{
	public:
		MeshRenderer();
		~MeshRenderer();

		inline void SetMesh(std::shared_ptr<Mesh> mesh) { m_Mesh = mesh; };
		inline std::shared_ptr<Mesh> GetMesh() { return m_Mesh; };


		// Inherited via Component
		virtual void AcceptInOrder(GameObjectVisitor & visitor) override;
		virtual void AcceptPreOrder(GameObjectVisitor & visitor) override;
		virtual void AcceptPostOrder(GameObjectVisitor & visitor) override;

	private:
		std::shared_ptr<Mesh> m_Mesh;
	};
}