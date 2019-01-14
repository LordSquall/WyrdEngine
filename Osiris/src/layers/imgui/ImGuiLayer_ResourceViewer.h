#include "core/export.h"
#include "core/Layer.h"

#include "core/Resources.h"

#include "ImGuiPlugin.h"

namespace Osiris
{
	class OSIRIS_API ImGuiLayer_ResourceViewer : public ImGuiPlugin
	{
	public:
		ImGuiLayer_ResourceViewer();
		~ImGuiLayer_ResourceViewer();

		inline Resources& GetResources() { return *m_Resources; }
		inline void SetResources(std::shared_ptr<Resources> resources) { m_Resources = resources; }

		void OnEditorRender() override;

	private:
		std::shared_ptr<Resources> m_Resources;
	};
}