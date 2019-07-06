#include "core/export.h"
#include "layers/Renderer2DLayer.h"

#include "editor/EditorPlugin.h"

namespace Osiris::Editor
{
	class SpriteLayerEditor : public EditorPlugin
	{
	public:
		SpriteLayerEditor();
		~SpriteLayerEditor();

		void OnEditorRender() override;

		inline void SetRenderingLayer(Layers::Renderer2DLayer* renderingLayer) { _RenderingLayer = renderingLayer; }

	private:
		Layers::Renderer2DLayer* _RenderingLayer;

		int32_t _SelectedSprite;
	};
}