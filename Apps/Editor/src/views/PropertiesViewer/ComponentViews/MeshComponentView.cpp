/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/ecs/Components.h>
#include <core/Application.h>

/* local includes */
#include "MeshComponentView.h"

#include "support/ImGuiUtils.h"

/* external includes */
#include <imgui.h>

namespace Wyrd::Editor
{
	COMPONENT_VIEW_FACTORY_REGISTER(MeshComponentView, "Mesh");

	void MeshComponentView::OnEditorRender(Entity e, void* data)
	{
		/* Cast to the correct component */
		MeshComponent* mesh = (MeshComponent*)data;
		
		ImGui::ColorPicker4("Pos ", (float*)&mesh->color);
	}
}