#include "core/export.h"
#include "editor/EditorPlugin.h"

#include "editor/events/EditorEvents.h"

namespace Osiris::Editor
{
	class PropertiesViewer : public EditorPlugin
	{
	public:
		enum Mode {
			None = 0, SpriteUI = 1
		};

	public:
		PropertiesViewer();
		~PropertiesViewer();

		/* EditorPlugin functions */
		void OnEditorRender() override;

	private:
		Mode _Mode;
		void OnSelectedSpriteChanged(Events::EventArgs& args);
		
		void DrawSpriteUI();

		static Sprite* _SelectedSprite;
	};
}