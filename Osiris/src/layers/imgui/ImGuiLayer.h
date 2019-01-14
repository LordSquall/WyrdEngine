#include "core/export.h"
#include "core/Layer.h"

#include "ImGuiPlugin.h"
#include "ImGuiLayer_ResourceViewer.h"

namespace Osiris
{
	class OSIRIS_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		
		void OnAttach() override;
		void OnDetach() override;
		void OnRender(std::shared_ptr<Renderer> renderer) override;
		void OnEvent(Event& event) override;
		
	private:
		void UpdateMouse();
		void UpdateCursor();

	private:
		ImGuiLayer_ResourceViewer m_ResourceViewer;
		float	m_Time;
	};
}