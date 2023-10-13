/* core wyrd includes */
#include <wyrdpch.h>

/* local includes */
#include "MaterialEditorView.h"
#include "support/ImGuiUtils.h"

namespace Wyrd::Editor
{
	MaterialEditorView::MaterialEditorView(EditorLayer* editorLayer) : EditorViewBase("Material Editor View", editorLayer)
	{
		_SettingsService = ServiceManager::Get<SettingsService>();
	}

	MaterialEditorView::~MaterialEditorView() {}

	void MaterialEditorView::SetMaterial(std::shared_ptr<Material> material)
	{ 
		_Material = material; 

		//_ModifiedMaterial.Copy(material);

	}

	void MaterialEditorView::OnEditorRender()
	{
		if (_Material != nullptr)
		{
			ImGui::Text(_Material->GetName().c_str());

			static ImGuiTableFlags tableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedSame | ImGuiTableFlags_NoHostExtendX;

			if (ImGui::BeginTable("_materialInputsTbl", 3, tableFlags))
			{

				ImGui::TableSetupColumn("Name");
				ImGui::TableSetupColumn("Type");
				ImGui::TableSetupColumn("Binding");

				ImGui::TableHeadersRow();
				ImGui::TableNextRow();

				for (auto const& inputProp : _Material->GetInputPropertyList())
				{
					ImGui::TableSetColumnIndex(0);
					ImGui::Text("%s", inputProp.first.c_str());

					ImGui::TableSetColumnIndex(1);
					ImGui::Text("%s", inputProp.second.type.c_str());

					ImGui::TableSetColumnIndex(2);
					ImGui::Text("%s", inputProp.second.binding.c_str());
					ImGui::TableNextRow();
				}
				ImGui::EndTable();
			}

			if (ImGui::Button("OK"))
			{
				Close();
			}
		}
	}
}