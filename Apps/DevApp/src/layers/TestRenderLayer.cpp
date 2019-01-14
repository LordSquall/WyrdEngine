#include "TestRenderLayer.h"

#include "graphics/Shader.h"

TestRenderLayer::TestRenderLayer(std::shared_ptr<Resources> resources) : Layer("Loading Layer")
{
	/* save reference to the renderer */
	m_Resources = resources;

}

void TestRenderLayer::OnRender(std::shared_ptr<Osiris::Renderer> renderer)
{
	/* current there is no scene tree and render tables, therefore we will just render the first mesh in the system as a test
	also we currently had direct access to the meshmap resources variables, i think this should be private and exposed with begin/end iterator functions */
	if (m_Resources->m_MeshMap.size() > 0)
	{
		renderer->RenderMesh(m_Resources->m_MeshMap.begin()->second);
	}
}