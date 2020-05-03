#include "core/export.h"
#include "layers/Renderer2DLayer.h"

#include "services/ServiceManager.h"
#include "EditorPlugin.h"

namespace Osiris::Editor
{
	class SpriteLayerEditor : public EditorPlugin
	{
	public:
		SpriteLayerEditor();
		~SpriteLayerEditor();

		void OnInitialise() override;
		void OnEditorRender() override;

	private:
		void OnSceneOpened(Events::EventArgs& args);

		void AddSpriteGO();

	private:
		std::shared_ptr<WorkspaceService> _WorkspaceService;
		std::shared_ptr<ResourceService> _ResourceService;
		int32_t _SelectedSprite;
		std::shared_ptr<Layer2D> _SelectedLayer2D;
	};
}