#pragma once

#include "core/export.h"
#include "core/Layer.h"

#include "editor/EditorPlugin.h"
#include "editor/support/IconLibrary.h"
#include "editor/support/Utils.h"

#include "editor/datamodels/Project.h"

#include "core/pipeline/Scene.h"
#include "core/pipeline/Sprite.h"

namespace Osiris::Editor::Events
{
	enum EventType {
		SelectedCleared, SelectedSpriteChanged,
		CreateNewProject, OpenProject, CloseProject, ProjectLoaded
	};

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

#pragma region CreateNewProjectEvent

	class CreateNewProjectArgs : public EventArgs
	{
	public:
		CreateNewProjectArgs(std::string name, std::string location) : name(name), location(location) {}

		const std::string name;
		const std::string location;
	};

	class CreateNewProjectEvent : public Event
	{
	public:
		CreateNewProjectEvent() : Event(EventType::CreateNewProject) { }
	};

#pragma endregion

#pragma region OpenProjectEvent

	class OpenProjectArgs : public EventArgs
	{
	public:
		OpenProjectArgs(std::string name, std::string location) : name(name), location(location) {}

		const std::string name;
		const std::string location;
	};

	class OpenProjectEvent : public Event
	{
	public:
		OpenProjectEvent() : Event(EventType::OpenProject) { }
	};

#pragma endregion

#pragma region CloseProjectEvent

	class CloseProjectArgs : public EventArgs
	{
	public:
		CloseProjectArgs(std::string name, std::string location) : name(name), location(location) {}

		const std::string name;
		const std::string location;
	};

	class CloseProjectEvent : public Event
	{
	public:
		CloseProjectEvent() : Event(EventType::CloseProject) { }
	};

#pragma endregion

#pragma region ProjectLoadedEvent

	class ProjectLoadedArgs : public EventArgs
	{
	public:
		ProjectLoadedArgs(std::string initialScene, std::shared_ptr<Project> project, std::string projectPath) : initialScene(initialScene), project(project), projectPath(projectPath) {}

		const std::string initialScene;
		std::shared_ptr<Project> project;
		const std::string projectPath;
	};

	class ProjectLoadedEvent : public Event
	{
	public:
		ProjectLoadedEvent() : Event(EventType::ProjectLoaded) { }
	};

#pragma endregion

}