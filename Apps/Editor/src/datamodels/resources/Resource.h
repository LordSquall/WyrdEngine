#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/export.h>
#include <core/UID.h>
#include <jsonxx.h>

/* local includes */
#include "ResourceTypes.h"
#include "support/IconLibrary.h"

#ifndef WYRD_PLATFORM_LINUX
typedef unsigned char BYTE;
#endif

typedef std::function<void()> OnResourceLoadSuccessful;
typedef std::function<void()> OnResourceLoadFailure;

namespace Wyrd::Editor
{
	/**
	 * @brief Editor Resource
	 * This class is provide a common interface for all resources used within the Editor application
	*/
	class Resource : public std::enable_shared_from_this<Resource>
	{
	public:
		enum IOResult
		{
			Success = 0, FileNotFound = 1, FileInUse = 2, FileMalformed = 3,
			DirectoryNotFound = 4, FileAlreadyExists = 5, InsufficientSpace = 6,
			NotImplemented = 7, Unknown = 8
		};

	public:
		Resource(const std::string& name, const UID& uid) : _name(name), _isLoaded(false), _path(), _resourceID(uid), _isEditorOnly(false) { }
		virtual ~Resource() = default;

		/**
		 * @brief Loads the content from a filepath
		 * @args filepath - absolute filepath
		 * @return result codes
		*/
		virtual IOResult Load(const std::string& filepath) = 0;

		/**
		 * @brief Loads the content from json object
		 * @args obj - json object
		 * @return result codes
		*/
		virtual IOResult Load(const jsonxx::Object& obj) = 0;
		
		/**
		 * @brief Save the content to disk
		 * @args filepath - absolute filepath
		 * @return result codes
		*/
		virtual IOResult Save(const std::string& filepath) = 0;

		/**
		 * @brief Reload the content and trigger any upward resolution
		 * @return result codes
		*/
		virtual IOResult Reload();

		/**
		 * @brief Optional function on resources to allow external references to be evaluated
		*/
		virtual void ResolveReferences() {};

		/**
		 * @brief Optional function on resources to allow trigger build operations once all resources are resolved
		*/
		virtual void Build(bool isRebuild = false) {};

		/**
		 * @brief Returns the resource tupe as defined in ResourceTypes.h
		 * @return Resource Type
		*/
		virtual ResourceType GetType() = 0;

		/**
		 * @brief Returns the unique type tag
		 * @return Resource tag
		*/
		virtual const std::string GetTypeTag() = 0;

		/**
		 * @brief Returns the Human readable name for the resource
		 * @return Resource name
		*/
		inline const std::string& GetName() const { return _name; }

		/**
		 * @brief Sets the Human readable name for the resource
		 * @args name - resource name
		*/
		inline void SetName(const std::string& name) { _name = name; }

		/**
		 * @brief Returns the base filepath used for the resource
		 * @return Filepath
		*/
		inline const std::filesystem::path& GetPath() const { return _path; }

		/**
		 * @brief Sets the Human readable name for the resource
		 * @args name - resource name
		*/
		inline void SetPath(const std::filesystem::path& path) { _path = path; }

		/**
		 * @brief Get the Icon of the resource Thumbnail
		*/
		virtual const Icon& GetThumbnail();

		/**
		 * @brief Returns the isLoaded flag
		 * @return isLoaded
		*/
		inline bool IsLoaded() const { return _isLoaded; }

		/**
		 * @brief Returns the Unique resource ID 
		 * @return Resource ID
		*/
		inline const UID GetResourceID() const { return _resourceID; }

		/**
		 * @brief Returns the flag to indicate if the resource is editor only
		 * @return Editor only flag
		*/
		inline bool IsEditorOnly() const { return _isEditorOnly; }

		/**
		 * @brief Sets the Editor only flag
		 * @args flag - editor only flag
		*/
		inline void IsEditorOnly(bool flag) { _isEditorOnly = flag; }

		inline void AddListeningResource(ResourceRef res) { _listeningResources.emplace(res->GetResourceID(), res); }

		inline void RemoveListeningResource(UID uid) { _listeningResources.erase(uid); }

		virtual void DrawProperties();

	protected:
		std::string _name;
		bool _isLoaded;
		std::filesystem::path _path;
		UID _resourceID;
		bool _isEditorOnly;

		OnResourceLoadSuccessful _onSuccessfulLoadCallback;
		OnResourceLoadFailure _onFailureLoadCallback;

		std::map<UID, ResourceRef> _listeningResources;
	};
}