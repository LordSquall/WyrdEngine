#include "core/export.h"
#include "layers/Renderer2DLayer.h"

#include "editor/EditorPlugin.h"
#include "editor/services/ResourceService.h"

namespace Osiris::Editor
{
	class AssetViewer : public EditorPlugin
	{
	public:
		AssetViewer();
		~AssetViewer();

		void OnEditorRender() override;

	private:
		void PopulateFolderNode(std::string& dir, std::string& name);

		void TextureItemGui();
		void ShaderItemGui();
		void ModelsItemGui();

		std::string _currentDir;
		std::shared_ptr<ResourceService> _resourcesService;
	};
}