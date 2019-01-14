#include "core/export.h"
#include "core/Layer.h"

namespace Osiris
{
	class ResourceLoaderLayer : public Layer
	{
	public:
		ResourceLoaderLayer();

		void OnUpdate() override;

		void OnEvent(Event& event) override;
	};
}