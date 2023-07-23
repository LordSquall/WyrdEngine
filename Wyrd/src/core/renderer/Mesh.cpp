#include "wyrdpch.h"
#include "core/renderer/Renderer.h"
#include "core/renderer/Mesh.h"

namespace Wyrd
{
	Mesh* Mesh::Create(const MeshDesc& desc)
	{
		Mesh* newMesh = new Mesh();

		newMesh->SetUID(desc.resource.guid);
		newMesh->SetName(desc.resource.name);

		newMesh->Vertices = desc.vertices;
		newMesh->boundingBox = desc.boundingBox;

		return newMesh;
	}
}