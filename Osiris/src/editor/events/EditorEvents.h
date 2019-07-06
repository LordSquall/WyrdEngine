#pragma once

#include "core/export.h"
#include "core/Layer.h"

#include "editor/EditorPlugin.h"
#include "editor/support/IconLibrary.h"
#include "editor/support/Utils.h"


#include "core/pipeline/Sprite.h"

namespace Osiris::Editor::Events
{
	enum EventType { SelectedCleared = 0, SelectedSpriteChanged = 1 };

	class Event
	{
	public:
		Event(EventType type) {}
		~Event() {}

		/* Getters and Setters */
		inline EventType GetType() { return _Type; }

	protected:
		EventType _Type;
	};

	class EventArgs
	{
	public:
		int keepalive;
	};

#pragma region SelectedClearedEvent

	class SelectedClearedEvent : public Event
	{
	public:
		SelectedClearedEvent() : Event(EventType::SelectedCleared) { }
	};

#pragma endregion

#pragma region SelectedSpriteChangedEvent

	class SelectedSpriteChangedArgs : public EventArgs
	{
	public:
		SelectedSpriteChangedArgs(const Sprite* sprite) : sprite(sprite) { keepalive = 99; }

		const Sprite* sprite;
	};

	class SelectedSpriteChangedEvent : public Event
	{
	public:
		SelectedSpriteChangedEvent() : Event(EventType::SelectedSpriteChanged) { }
	};

#pragma endregion

}