#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/export.h>
#include <core/UID.h>

/* local includes */
#include "ResourceTypes.h"

#ifndef WYRD_PLATFORM_LINUX
typedef unsigned char BYTE;
#endif

namespace Wyrd::Editor
{
	/**
	 * @brief Editor Resource
	 * This class is provide a common interface for all resources used within the Editor application
	*/
	class Resource
	{
	public:
		Resource(const std::string& name, const std::filesystem::path& path, const UID& uid) : _name(name), _isLoaded(false), _path(path), _resourceID(uid) { }
		virtual ~Resource() = default;

		/**
		 * @brief Loads the content from disk using the file path specified in the Resource Constructor
		 * @return 0 is the resource loaded successfully, otherwise resource specific result codes
		*/
		virtual int Load() = 0;

		/**
		 * @brief Optional function on resources to allow external references to be evaluated
		*/
		virtual void ResolveReferences() {};

		/**
		 * @brief Returns the resource tupe as defined in ResourceTypes.h
		 * @return Resource Type
		*/
		virtual ResourceType GetType() = 0;

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
		 * @brief Returns the isLoaded flag
		 * @return isLoaded
		*/
		inline bool IsLoaded() const { return _isLoaded; }

		/**
		 * @brief Returns the Unique resource ID 
		 * @return Resource ID
		*/
		inline const UID GetResourceID() const { return _resourceID; }


	protected:
		std::string _name;
		bool _isLoaded;
		std::filesystem::path _path;
		UID _resourceID;
	};
}