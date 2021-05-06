#pragma once

/* local includes  */
#include "extensions/systems/EditorComponent.h"

namespace Wyrd::Editor {


	bool EditorComponent::Initialise()
	{
		return true;
	}


	void EditorComponent::Recalculate()
	{

	}


	bool EditorComponent::ToJson(jsonxx::Object& object) { return true; }
	bool EditorComponent::FromJson(jsonxx::Object& object) { return true; }
}