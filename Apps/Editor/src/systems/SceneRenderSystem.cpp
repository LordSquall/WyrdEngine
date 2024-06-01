#include <wyrdpch.h>

#include <core/Log.h>
#include <core/ecs/ECS.h>
#include <core/ecs/EntitySet.h>
#include <core/ecs/Components.h>
#include <core/Application.h>

#include "systems/SceneRenderSystem.h"
#include "datamodels/CameraController.h"

namespace Wyrd::Editor
{
	bool SceneRenderSystem::OnUpdate(ISystemContext& context)
	{
		SceneRenderSystemContext& systemCtx = (SceneRenderSystemContext&)context;

		for (Entity e : EntitySet<Transform3DComponent, MeshRendererComponent, MaterialComponent>(systemCtx.scene))
		{
			Transform3DComponent* transform = systemCtx.scene.Get<Transform3DComponent>(e);
			MeshRendererComponent* meshRenderer = systemCtx.scene.Get<MeshRendererComponent>(e);
			MaterialComponent* material = systemCtx.scene.Get<MaterialComponent>(e);

			Wyrd::DrawMeshCommand cmd{};
			cmd.modelMatrix = transform->modelMatrix * transform->parentModelMatrix;
			cmd.viewMatrix = systemCtx.cameraController.GetCamera().GetViewMatrix();
			cmd.projectionMatrix = systemCtx.cameraController.GetCamera().GetProjectionMatrix();
			cmd.material = Application::Get().GetResources().Materials[material->material].get();
			cmd.materialProps = &material->properties;
			cmd.mesh = Application::Get().GetResources().Meshs[meshRenderer->model].get();
			cmd.baseTexture = Application::Get().GetResources().Textures[RES_TEXTURE_DEFAULT].get();
			cmd.drawType = RendererDrawType::Triangles;
			
			systemCtx.renderer.Submit(cmd);
			systemCtx.renderer.Flush();
		}
		return true;
	}
}