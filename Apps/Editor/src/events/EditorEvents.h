#pragma once

/* core osiris includes */
#include <core/export.h>
#include <core/Layer.h>
#include <core/scene/Scene.h>
#include <core/scene/GameObject.h>

/* local includes */
#include "views/EditorViewBase.h"
#include "support/IconLibrary.h"
#include "support/Utils.h"
#include "datamodels/Project.h"
#include "datamodels/resources/Resource.h"
#include "datamodels/resources/ResourceFactory.h"
#include "datamodels/logging/LogMessage.h"

#define EVENT_ARGS_CLONE(type)	public:  std::shared_ptr<EventArgs> Clone() { return std::make_shared<type>(*this); } 

namespace Osiris::Editor::Events
{
	enum class EventType {
		SettingsUpdated,
		SelectedCleared, SelectedGameObjectChanged, SelectedAssetChanged,
		CreateNewProject, OpenProject, CloseProject, ProjectLoaded,
		SceneClosed, SceneOpened,
		OpenSceneViewerContextMenu,
		AddLogEntry, ClearLogEntry,
		AddFileEntry, DeleteFileEntry, ModifiedFileEntry, RenameFileEntry,
		AddResource, DeleteResource, ReloadResource, RenameResource,
		LoadAsset, BuildBehaviourModel
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

	class EventArgs { 
	public:
		virtual std::shared_ptr<EventArgs> Clone() = 0;
	};

#pragma region SettingsUpdatedEvent

	class SettingsUpdateEventArgs : public EventArgs
	{
	public:
		SettingsUpdateEventArgs() {}

		EVENT_ARGS_CLONE(SettingsUpdateEventArgs)
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
		SelectedGameObjectChangedArgs(Osiris::GameObject* gameObject) : gameObject(gameObject) { }

		Osiris::GameObject* gameObject;

		EVENT_ARGS_CLONE(SelectedGameObjectChangedArgs)
	};

	class SelectedGameObjectChangedEvent : public Event
	{
	public:
		SelectedGameObjectChangedEvent() : Event(EventType::SelectedGameObjectChanged) { }
	};

#pragma endregion

#pragma region SelectedAssetChangedEvent

	class SelectedAssetChangedArgs : public EventArgs
	{
	public:
		SelectedAssetChangedArgs(const std::shared_ptr<Resource> resource) : resource(resource) { }

		const std::shared_ptr<Resource> resource;

		EVENT_ARGS_CLONE(SelectedAssetChangedArgs)
	};

	class SelectedAssetChangedEvent : public Event
	{
	public:
		SelectedAssetChangedEvent() : Event(EventType::SelectedAssetChanged) { }
	};

#pragma endregion

#pragma region CreateNewProjectEvent

	class CreateNewProjectArgs : public EventArgs
	{
	public:
		CreateNewProjectArgs(std::string name, std::string scene, std::string location) : name(name), sceneName(scene), location(location) {}

		const std::string name;
		const std::string sceneName;
		const std::string location;

		EVENT_ARGS_CLONE(CreateNewProjectArgs)
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

		EVENT_ARGS_CLONE(OpenProjectArgs)
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

		EVENT_ARGS_CLONE(CloseProjectArgs)
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

		EVENT_ARGS_CLONE(ProjectLoadedArgs)
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
		SceneClosedArgs() {}

		EVENT_ARGS_CLONE(SceneClosedArgs)
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

		EVENT_ARGS_CLONE(SceneOpenedArgs)
	};

	class SceneOpenedEvent : public Event
	{
	public:
		SceneOpenedEvent() : Event(EventType::SceneOpened) { }
	};

#pragma endregion

#pragma region OpenSceneViewerContextMenu

	class OpenSceneViewerContextMenuArgs : public EventArgs
	{
	public:
		OpenSceneViewerContextMenuArgs(std::shared_ptr<GameObject> gameObject) : gameobject(gameObject) {}

		std::shared_ptr<GameObject> gameobject;

		EVENT_ARGS_CLONE(OpenSceneViewerContextMenuArgs)
	};

	class OpenSceneViewerContextMenuEvent : public Event
	{
	public:
		OpenSceneViewerContextMenuEvent() : Event(EventType::OpenSceneViewerContextMenu) { }
	};

#pragma endregion

#pragma region AddLogEntry

	class AddLogEntryArgs : public EventArgs
	{
	public:
		AddLogEntryArgs(const AddLogEntryArgs& obj)
		{
			this->type = obj.type;
			this->severity = obj.severity;
			this->msg = obj.msg;
		}

		AddLogEntryArgs(LogType type, Severity severity, const std::string& message)
		{
			this->type = type;
			this->severity = severity;
			this->msg = message;
		}

		LogType		type;
		Severity	severity;
		std::string msg;

		EVENT_ARGS_CLONE(AddLogEntryArgs)
	};

	class AddLogEntryEvent : public Event
	{
	public:
		AddLogEntryEvent() : Event(EventType::AddLogEntry) { }
	};

#pragma endregion

#pragma region ClearLogEntry

	class ClearLogEntryArgs : public EventArgs
	{
	public:
		ClearLogEntryArgs(const ClearLogEntryArgs& obj)
		{
			this->type = obj.type;
		}

		ClearLogEntryArgs(LogType type)
		{
			this->type = type;
		}

		LogType		type;

		EVENT_ARGS_CLONE(ClearLogEntryArgs)
	};

	class ClearLogEntryEvent : public Event
	{
	public:
		ClearLogEntryEvent() : Event(EventType::ClearLogEntry) { }
	};

#pragma endregion

#pragma region AddFileEntry

	class AddFileEntryArgs : public EventArgs
	{
	public:
		AddFileEntryArgs(const std::string dir, const std::string filepath, const bool isDir) : dir(dir), filepath(filepath), isDir(isDir) {}

		std::string dir;
		std::string filepath;
		bool isDir;

		EVENT_ARGS_CLONE(AddFileEntryArgs)
	};

	class AddFileEntryEvent : public Event
	{
	public:
		AddFileEntryEvent() : Event(EventType::AddFileEntry) { }
	};

#pragma endregion

#pragma region DeleteFileEntry

	class DeleteFileEntryArgs : public EventArgs
	{
	public:
		DeleteFileEntryArgs(const std::string dir, const std::string filepath, const bool isDir) : dir(dir), filepath(filepath), isDir(isDir) {}

		std::string dir;
		std::string filepath;
		bool isDir;

		EVENT_ARGS_CLONE(DeleteFileEntryArgs)
	};

	class DeleteFileEntryEvent : public Event
	{
	public:
		DeleteFileEntryEvent() : Event(EventType::DeleteFileEntry) { }
	};

#pragma endregion

#pragma region ModifiedFileEntry

	class ModifiedFileEntryArgs : public EventArgs
	{
	public:
		ModifiedFileEntryArgs(const std::string dir, const std::string filepath, const bool isDir) : dir(dir), filepath(filepath), isDir(isDir) {}

		std::string dir;
		std::string filepath;
		bool isDir;

		EVENT_ARGS_CLONE(ModifiedFileEntryArgs)
	};

	class ModifiedFileEntryEvent : public Event
	{
	public:
		ModifiedFileEntryEvent() : Event(EventType::ModifiedFileEntry) { }
	};

#pragma endregion


#pragma region RenameFileEntry

	class RenameFileEntryArgs : public EventArgs
	{
	public:
		RenameFileEntryArgs(const std::string dir, const std::string filepath, const std::string previousName, const bool isDir) : dir(dir), filepath(filepath), previousName(previousName), isDir(isDir) {}

		std::string dir;
		std::string filepath;
		std::string previousName;
		bool isDir;

		EVENT_ARGS_CLONE(RenameFileEntryArgs)
	};

	class RenameFileEntryEvent : public Event
	{
	public:
		RenameFileEntryEvent() : Event(EventType::RenameFileEntry) { }
	};

#pragma endregion

#pragma region AddResource

	class AddResourceArgs : public EventArgs
	{
	public:
		AddResourceArgs(const std::string filepath) : filepath(filepath) {}

		std::string filepath;

		EVENT_ARGS_CLONE(AddResourceArgs)
	};

	class AddResourceEvent : public Event
	{
	public:
		AddResourceEvent() : Event(EventType::AddResource) { }
	};

#pragma endregion

#pragma region DeleteResource

	class DeleteResourceArgs : public EventArgs
	{
	public:
		DeleteResourceArgs(UID uuid) : uid(uid) {}

		UID uid;

		EVENT_ARGS_CLONE(DeleteResourceArgs)
	};

	class DeleteResourceEvent : public Event
	{
	public:
		DeleteResourceEvent() : Event(EventType::DeleteResource) { }
	};

#pragma endregion

#pragma region ReloadResource

	class ReloadResourceArgs : public EventArgs
	{
	public:
		ReloadResourceArgs(UID uuid) : uid(uid) {}

		UID uid;
		
		EVENT_ARGS_CLONE(ReloadResourceArgs)
	};

	class ReloadResourceEvent : public Event
	{
	public:
		ReloadResourceEvent() : Event(EventType::ReloadResource) { }
	};

#pragma endregion

#pragma region RenameResource

	class RenameResourceArgs : public EventArgs
	{
	public:
		RenameResourceArgs(const std::string& newName, UID uid) : newName(newName), uid(uid) {}

		std::string newName;
		UID uid;

		EVENT_ARGS_CLONE(RenameResourceArgs)
	};

	class RenameResourceEvent : public Event
	{
	public:
		RenameResourceEvent() : Event(EventType::RenameResource) { }
	};

#pragma endregion

#pragma region LoadAsset

	class LoadAssetArgs : public EventArgs
	{
	public:
		LoadAssetArgs(Resource* resource) : resource(resource) {}

		Resource* resource;

		EVENT_ARGS_CLONE(LoadAssetArgs)
	};

	class LoadAssetEvent : public Event
	{
	public:
		LoadAssetEvent() : Event(EventType::LoadAsset) { }
	};

#pragma endregion

#pragma region BuildBehaviourModel

	class BuildBehaviourModelArgs : public EventArgs
	{
	public:
		BuildBehaviourModelArgs() {}

		EVENT_ARGS_CLONE(BuildBehaviourModelArgs)
	};

	class BuildBehaviourModelEvent : public Event
	{
	public:
		BuildBehaviourModelEvent() : Event(EventType::BuildBehaviourModel) { }
	};

#pragma endregion
}