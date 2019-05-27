#include "osrpch.h"

#include "MeshRenderer.h"

Osiris::MeshRenderer::MeshRenderer()
{
}

Osiris::MeshRenderer::~MeshRenderer()
{
}

void Osiris::MeshRenderer::AcceptInOrder(GameObjectVisitor & visitor)
{
	visitor.Visit(this);
}

void Osiris::MeshRenderer::AcceptPreOrder(GameObjectVisitor & visitor)
{
	visitor.Visit(this);
}

void Osiris::MeshRenderer::AcceptPostOrder(GameObjectVisitor & visitor)
{
	visitor.Visit(this);
}