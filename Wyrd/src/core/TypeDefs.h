#pragma once

#include "properties/BaseProp.h"
#include "core/behaviour/ScriptedClass.h"
#include "core/pipeline/materials/Material.h"

#include <string>
#include <map>
#include <memory>

namespace Wyrd
{
	//typedef std::unique_ptr<BaseProp> BasePropRef;
	//typedef std::vector<BaseProp> BasePropList_t;
	//typedef std::vector<BaseProp> BasePropMap_t;

	//typedef std::shared_ptr<BasePropList_t> PropetryListRef;
	//typedef std::shared_ptr<BasePropMap_t> PropetryMapRef;

	typedef std::shared_ptr<PropertyList_t> ScriptPropertiesMapRef;
	typedef std::shared_ptr<DataList_t> ScriptPropertiesDataMapRef;
}