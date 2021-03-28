
/* core osiris includes */
#include <core/export.h>
#include <events/KeyEvent.h>

/* local includes */
#include "views/EditorViewBase.h"
#include "services/ServiceManager.h"

namespace Osiris::Editor
{
	class AssetViewer : public EditorViewBase
	{
	public:
		struct DirectoryEntry_s;

		struct DirectoryEntry_s
		{
			uint32_t id;
			bool open;
			std::string name;
			std::string dir;
			std::vector<std::shared_ptr<DirectoryEntry_s>> subdirs;
			std::map<UID, std::shared_ptr<Resource>> files;
			std::shared_ptr<DirectoryEntry_s> parent;
		};

	public:
		AssetViewer(EditorLayer* editorLayer);
		~AssetViewer() = default;

		void OnEditorRender() override;
		void OnEvent(Event& event) override;

		void Refresh();	
		std::shared_ptr<AssetViewer::DirectoryEntry_s> RefreshSubDir(const std::string& folder, std::shared_ptr<AssetViewer::DirectoryEntry_s> parent);

		void SetCurrentSelectedDirectory(std::shared_ptr<AssetViewer::DirectoryEntry_s> directory);
		std::shared_ptr<AssetViewer::DirectoryEntry_s> GetCurrentSelectedDirectory();

	private:
		bool OnKeyPressedEvent(KeyPressedEvent& e);

		bool DirectoryTreeViewRecursive(std::shared_ptr<AssetViewer::DirectoryEntry_s> dirEntry);
		void DrawDirectoryContextMenu(uint32_t nodePtr);
		bool DrawEditNode();

		void DrawAssetsItems();

		void DrawTextureItem(uint32_t resIdx, TextureRes& textureResource);
		void DrawSceneItem(uint32_t resIdx, SceneRes& sceneResource);
		void DrawScriptItem(uint32_t resIdx, ScriptRes& scriptResource);
		void DrawUnknownItem(uint32_t resIdx, std::string& unknownResourceName);

		/* Directory State */
		std::shared_ptr<AssetViewer::DirectoryEntry_s> _currentSelectedDir;
		uint32_t _currentContextNodeIdx;
		bool _refreshing;
		
		/* Resource State */
		UID _currentSelectedResourceUID;

		/* Services*/
		std::shared_ptr<ResourceService> _resourcesService;
		std::shared_ptr<WorkspaceService> _workspaceService;
		std::shared_ptr<EventService> _EventService;
		std::shared_ptr<DialogService> _dialogService;

		/* Icons */
		std::shared_ptr<Icon> _UnknownIcon;
		std::shared_ptr<Icon> _SceneIcon;
		std::shared_ptr<Icon> _TextureIcon;
		std::shared_ptr<Icon> _ScriptIcon;
	};
}