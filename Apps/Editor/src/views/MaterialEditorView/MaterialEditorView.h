#include "core/export.h"
#include "core/LayerStack.h"
#include "core/renderer/Material.h"
#include "services/ServiceManager.h"

#include "views/EditorViewBase.h"

namespace Wyrd::Editor
{
	class MaterialEditorView : public EditorViewBase
	{
	public:
		MaterialEditorView(EditorLayer* editorLayer);
		~MaterialEditorView();

		virtual void OnEditorRender() override;

		void SetMaterial(std::shared_ptr<Material> material);

	private:
		std::shared_ptr<SettingsService> _SettingsService;
		std::shared_ptr<Material> _Material;

		Material _ModifiedMaterial;
	};
}