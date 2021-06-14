/* core wyrd includes */
#include <core/export.h>
#include <core/ecs/ECS.h>

/* local includes */
#include "views/EditorViewBase.h"
#include "events/EditorEvents.h"
#include "datamodels/resources/Resource.h"

#define COMPONENT_VIEW_FACTORY_REGISTER(clsname, name)\
bool clsname::s_Registered = ComponentViewFactory::Register(name, &clsname::OnEditorRender)


#define COMPONENT_VIEW_FACTORY_SETUP(clsname) \
public:\
clsname() {}\
public:\
	static void OnEditorRender(Entity e, void* data); \
private:\
	static bool s_Registered


namespace Wyrd::Editor
{
	class WYRD_LIBRARY_API ComponentViewFactory
	{
	public:
		using ViewRenderFunc = void(*)(Entity e, void* data);

	public:
		ComponentViewFactory() = default;

		static bool Register(const std::string name, ViewRenderFunc createFunc);

		static void Create(const std::string& name, Entity e, void* data);
	};
}