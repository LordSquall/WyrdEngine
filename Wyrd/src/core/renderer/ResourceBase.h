#pragma once

/* local includes */
#include "core/export.h"
#include "core/UID.h"

/* external includes */
#include <string>

namespace Wyrd
{
	struct ResourceDesc
	{
		Wyrd::UID guid;
		std::string name;

		ResourceDesc() : guid(), name("") {}
	};

	class WYRD_LIBRARY_API ResourceBase
	{
	public:
		virtual ~ResourceBase() {}

		inline const UID GetUID() const { return _uid; }
		inline void SetUID(const UID uid) { _uid = uid; }

		inline const std::string GetName() const { return _name; }
		inline void SetName(const std::string& name) { _name = name; }

	protected:
		UID _uid;
		std::string _name;
	};
}