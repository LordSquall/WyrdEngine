#include "core/export.h"
#include "core/Layer.h"

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
		float	m_Time;
	};
}