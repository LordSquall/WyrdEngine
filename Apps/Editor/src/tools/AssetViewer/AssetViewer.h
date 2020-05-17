#include "core/export.h"
#include "layers/Renderer2DLayer.h"

#include "EditorPlugin.h"
#include "services/ServiceManager.h"

namespace Osiris::Editor
{
	class AssetViewer : public EditorPlugin
	{
	public:
		AssetViewer();
		~AssetViewer();

		void OnEditorRender() override;

	private:
		void PopulateFolderNode(const std::string& dir);

		void DrawAssetsItems();

		void DrawTextureItem(uint32_t resIdx, std::shared_ptr<TextureRes> textureResource);
		void DrawSceneItem(uint32_t resIdx, std::shared_ptr<SceneRes> sceneResource);
		void DrawUnknownItem(uint32_t resIdx, std::string& unknownResourceName);

		std::string _currentDir;
		std::shared_ptr<ResourceService> _resourcesService;
		std::shared_ptr<WorkspaceService> _workspaceService;

		/* Icons */
		std::shared_ptr<Icon> _UnknownIcon;
		std::shared_ptr<Icon> _SceneIcon;
		std::shared_ptr<Icon> _TextureIcon;
	};
}