#pragma once

#include "osrpch.h"

#include "SpriteLayerEditor.h"

#include "core/Application.h"
#include "core/Layer.h"


#include "imgui.h"

namespace Osiris::Editor
{
	SpriteLayerEditor::SpriteLayerEditor() : EditorPlugin("Sprite Layer Editor"), _SelectedLayerIndex(-1), _LayerSelected(false) {}

	SpriteLayerEditor::~SpriteLayerEditor() {}

	void SpriteLayerEditor::OnEditorRender()
	{
		ImGui::Begin("Sprite Layer Editor");

		if (ImGui::TreeNode("Sprite Layers"))
		{
			for (auto& item : _RenderingLayer->GetSpriteLayers())
			{
				if (ImGui::TreeNode(item->GetName().c_str()))
				{
					for (auto& sprite : item->GetSprites())
					{
						if (ImGui::Selectable(sprite->GetName().c_str()) == true)
						{
						}
					}
					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}

		if (ImGui::Button("+") == true)
		{
			_RenderingLayer->AddSpriteLayer("New Layer " + std::to_string(_RenderingLayer->GetSpriteLayers().size()));
		}
		ImGui::SameLine();
		if (ImGui::Button("-") == true)
		{
			if (_LayerSelected == true)
			{
				_RenderingLayer->RemoveSpriteLayer(_SelectedLayerIndex);
				_SelectedLayerIndex = _SelectedLayerIndex - 1;

				if (_SelectedLayerIndex < 0) _SelectedLayerIndex = 0;

				_LayerSelected = true;
			}

			if (_RenderingLayer->GetSpriteLayers().size() == 0)
			{
				_LayerSelected = false;
			}
		}
		
		ImGui::End();
	}

}