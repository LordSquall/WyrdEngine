#include "core/export.h"
#include "core/LayerStack.h"

#include "services/ServiceManager.h"

#include "views/EditorViewDialogBase.h"

namespace Wyrd::Editor
{
	class SpriteSheetImporterDialog : public EditorViewDialogBase
	{
	public:
		SpriteSheetImporterDialog(EditorLayer* editorLayer, TextureRes* textureResource);
		~SpriteSheetImporterDialog();

		virtual void OnDialogRender();

		TextureRes* _TextureResource;

	public:
		int32_t _SelectedImageAreaID;
		std::vector<ImageArea> _SubImageData;
	};
}