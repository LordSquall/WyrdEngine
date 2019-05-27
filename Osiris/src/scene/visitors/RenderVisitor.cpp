#include "osrpch.h"
#include "RenderVisitor.h"

namespace Osiris {

	RenderVisitor::RenderVisitor()
	{

	}

	RenderVisitor::~RenderVisitor()
	{

	}

	void RenderVisitor::Visit(GameObject * gameObject)
	{
		OSR_INFO("RenderVisitor::Visit(GameObject)");
	}

	void RenderVisitor::Visit(MeshRenderer * meshRenderer)
	{
		OSR_INFO("RenderVisitor::Visit(MeshRenderer)");
	}

}