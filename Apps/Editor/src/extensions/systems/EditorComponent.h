#pragma once

/* local includes  */
#include "core/export.h"
#include "core/Timestep.h"
#include "core/scene/components/IBaseComponent.h"

namespace Wyrd::Editor {

	class WYRD_LIBRARY_API EditorComponent : public IBaseComponent
	{
	public:
		EditorComponent(GameObject* gameObject) : IBaseComponent(gameObject, SceneComponentType::CustomComponent) {}
		virtual ~EditorComponent() {};

		virtual bool Initialise() override;
		virtual void Recalculate() override;

		virtual bool ToJson(jsonxx::Object& object) override;
		virtual bool FromJson(jsonxx::Object& object) override;

		virtual const std::string GetManagedType() { return nullptr; };

		Rect inputArea;
	};
}