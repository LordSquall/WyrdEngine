#include "core/export.h"
#include "layers/Renderer2DLayer.h"

#include "views/EditorViewBase.h"
#include "services/ServiceManager.h"

namespace Osiris::Editor
{
	class AssetViewer : public EditorViewBase
	{
	public:
		AssetViewer(EditorLayer* editorLayer);
		~AssetViewer();

		void OnEditorRender() override;

	private:
		std::pair<bool, const std::string> DirectoryTreeViewRecursive(const std::filesystem::path& path, uint32_t* count, int* selection_mask, int depth = 0);
		void DrawDirectoryContextMenu(const std::string& dir);

		void DrawAssetsItems();

		void DrawTextureItem(uint32_t resIdx, std::shared_ptr<TextureRes> textureResource);
		void DrawSceneItem(uint32_t resIdx, std::shared_ptr<SceneRes> sceneResource);
		void DrawScriptItem(uint32_t resIdx, std::shared_ptr<ScriptRes> scriptResource);
		void DrawUnknownItem(uint32_t resIdx, std::string& unknownResourceName);

		std::string _currentDir;
		std::shared_ptr<ResourceService> _resourcesService;
		std::shared_ptr<WorkspaceService> _workspaceService;
		std::shared_ptr<EventService> _EventService;

		/* Icons */
		std::shared_ptr<Icon> _UnknownIcon;
		std::shared_ptr<Icon> _SceneIcon;
		std::shared_ptr<Icon> _TextureIcon;
		std::shared_ptr<Icon> _ScriptIcon;
	};
}