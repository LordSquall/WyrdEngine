#include "core/export.h"
#include "core/LayerStack.h"

#include "services/ServiceManager.h"

#include "views/EditorViewDialogBase.h"

namespace Wyrd::Editor
{
	//** TEMP */
	/*
	**********uv2
	*		  *
	*		  *
	*		  *
	*		  *
	*		  *
	uv1********
	*/
	struct ImageArea
	{
		int32_t id;
		std::string name;
		Vector2 uv1;
		Vector2 uv2;
	};

	class SpriteSheetImporterDialog : public EditorViewDialogBase
	{
	public:
		SpriteSheetImporterDialog(EditorLayer* editorLayer, TextureRes* textureResource);
		~SpriteSheetImporterDialog();

		virtual void OnDialogRender();

		TextureRes* _TextureResource;

	public:
		int32_t _SelectedImageAreaID;
	};
}