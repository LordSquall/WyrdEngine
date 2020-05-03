#include "core/export.h"
#include "EditorPlugin.h"

#include "events/EditorEvents.h"

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
		void OnSelectedGameObjectChanged(Events::EventArgs& args);
		
		void DrawSpriteUI();

		static std::shared_ptr<GameObject> _SelectedGameObject;
	};
}