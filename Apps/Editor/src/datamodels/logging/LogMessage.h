#pragma once

#include "osrpch.h"

#include "core/export.h"

namespace Osiris::Editor
{
	class LogMessage
	{
	public:
		enum Serverity
		{
			Info, Warning, Error, Debug
		};

		enum Type
		{
			Information,
			Script
			// e.g. ResourceImport
		};

	public:
		LogMessage() : _uid(_nextUid)
		{
			_nextUid++;
		}

		virtual ~LogMessage() {};

		// Getters and Setters
		inline uint32_t GetUID() { return _uid; }

	public:
		Type type;
		Serverity serverity;

	private:
		uint32_t _uid = 0u;

		static uint32_t _nextUid;
	};
}