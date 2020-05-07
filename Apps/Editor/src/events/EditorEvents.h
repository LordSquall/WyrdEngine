#pragma once

#include "core/export.h"
#include "core/Layer.h"

#include "EditorPlugin.h"
#include "support/IconLibrary.h"
#include "support/Utils.h"

#include "datamodels/Project.h"
#include "datamodels/Scene.h"

#include "core/pipeline/Sprite.h"

namespace Osiris::Editor::Events
{
	enum class EventType {
		SettingsUpdated,
		SelectedCleared, SelectedGameObjectChanged,
		CreateNewProject, OpenProject, CloseProject, ProjectLoaded,
		SceneClosed, SceneOpened
	};

	class Event
	{
	public:
		Event(EventType type) : _Type(type) {}
		~Event() {}

		/* Getters and Setters */
		inline EventType GetType() { return _Type; }

	protected:
		EventType _Type;
	};

	class EventArgs { };

#pragma region SettingsUpdatedEvent

	class SettingsUpdateEventArgs : public EventArgs
	{
	public:
		SettingsUpdateEventArgs() {}
	};

	class SettingsUpdateEvent : public Event
	{
	public:
		SettingsUpdateEvent() : Event(EventType::SettingsUpdated) { }
	};

#pragma endregion

#pragma region SelectedClearedEvent

	class SelectedClearedEvent : public Event
	{
	public:
		SelectedClearedEvent() : Event(EventType::SelectedCleared) { }
	};

#pragma endregion

#pragma region SelectedGameObjectChangedEvent

	class SelectedGameObjectChangedArgs : public EventArgs
	{
	public:
		SelectedGameObjectChangedArgs(const std::shared_ptr<GameObject> gameObject) : gameObject(gameObject) { }

		const std::shared_ptr<GameObject> gameObject;
	};

	class SelectedGameObjectChangedEvent : public Event
	{
	public:
		SelectedGameObjectChangedEvent() : Event(EventType::SelectedGameObjectChanged) { }
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

#pragma region SceneClosedEvent

	class SceneClosedArgs : public EventArgs
	{
	public:
		SceneClosedArgs(std::string name, std::string location) : name(name), location(location) {}

		const std::string name;
		const std::string location;
	};

	class SceneClosedEvent : public Event
	{
	public:
		SceneClosedEvent() : Event(EventType::SceneClosed) { }
	};

#pragma endregion

#pragma region SceneOpenedEvent

	class SceneOpenedArgs : public EventArgs
	{
	public:
		SceneOpenedArgs(std::shared_ptr<Scene> scene) : scene(scene) {}


		std::shared_ptr<Scene> scene;
	};

	class SceneOpenedEvent : public Event
	{
	public:
		SceneOpenedEvent() : Event(EventType::SceneOpened) { }
	};

#pragma endregion
}