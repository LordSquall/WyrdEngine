#include "wyrdpch.h"

/* local includes  */
#include "core/UID.h"
#include "core/Application.h"
#include "core/Resources.h"
#include "properties/BaseProp.h"
#include "serial/TypeSerialisers.h"

namespace Wyrd
{
	// templated setter functions 
	PROPERTY_TEMPLATE_SET_FUNC(Int, int)
	//PROPERTY_TEMPLATE_SET_FUNC(Int32, int32_t)	// not needed as int speciailisation function will catch this type
	PROPERTY_TEMPLATE_SET_FUNC(UInt32, uint32_t)
	PROPERTY_TEMPLATE_SET_FUNC(Int64, int64_t)
	PROPERTY_TEMPLATE_SET_FUNC(UInt64, uint64_t)
	PROPERTY_TEMPLATE_SET_FUNC(Float, float)
	PROPERTY_TEMPLATE_SET_FUNC(Double, double)
	PROPERTY_TEMPLATE_SET_FUNC(String, std::string)
	PROPERTY_TEMPLATE_SET_FUNC(Vec2, Vector2)
	PROPERTY_TEMPLATE_SET_FUNC(Vec3, Vector3)
	PROPERTY_TEMPLATE_SET_FUNC(Bool, bool)
	PROPERTY_TEMPLATE_SET_FUNC(Color, Color)
	PROPERTY_TEMPLATE_SET_FUNC(Texture, Texture*)
	//PROPERTY_TEMPLATE_SET_FUNC(Entity, Entity)   // not needed as uint64 speciailisation function will catch this type

	PROPERTY_TEMPLATE_GET_FUNC(Int, int)
	//PROPERTY_TEMPLATE_SET_FUNC(Int32, int32_t)	// not needed as int speciailisation function will catch this type
	PROPERTY_TEMPLATE_GET_FUNC(UInt32, uint32_t)
	PROPERTY_TEMPLATE_GET_FUNC(Int64, int64_t)
	PROPERTY_TEMPLATE_GET_FUNC(UInt64, uint64_t)
	PROPERTY_TEMPLATE_GET_FUNC(Float, float)
	PROPERTY_TEMPLATE_GET_FUNC(Double, double)
	PROPERTY_TEMPLATE_GET_FUNC(String, std::string)
	PROPERTY_TEMPLATE_GET_FUNC(Vec2, Vector2)
	PROPERTY_TEMPLATE_GET_FUNC(Vec3, Vector3)
	PROPERTY_TEMPLATE_GET_FUNC(Bool, bool)
	PROPERTY_TEMPLATE_GET_FUNC(Color, Color)
	PROPERTY_TEMPLATE_GET_FUNC(Texture, Texture*)
	//PROPERTY_TEMPLATE_GET_FUNC(Entity, Entity)   // not needed as uint64 speciailisation function will catch this type

	// Serialisation functions
	
	// Int
	PROPERTY_SERIALISE_FUNCSIG(Int)
	{
		jsonxx::Object obj;
		obj << "type" << "Int";
		obj << "value" << Value;
		json << _Name << obj;
		return true;
	}

	PROPERTY_DESERIALISE_FUNCSIG(Int)
	{
		Value = json.get<jsonxx::Number>("value", 0);
		return true;
	}

	PROPERTY_DESERIALISE_VAL_FUNCSIG(Int)
	{
		Value = json.get<jsonxx::Number>();
		return true;
	}

	PROPERTY_TOSTRING_FUNCSIG(Int) { return std::to_string(Value); }

	// Int32
	PROPERTY_SERIALISE_FUNCSIG(Int32)
	{
		jsonxx::Object obj;
		obj << "type" << "Int32";
		obj << "value" << Value;
		json << _Name << obj;
		return true;
	}

	PROPERTY_DESERIALISE_FUNCSIG(Int32)
	{
		Value = json.get<jsonxx::Number>("value", 0);
		return true;
	}
	
	PROPERTY_DESERIALISE_VAL_FUNCSIG(Int32)
	{
		Value = json.get<jsonxx::Number>();
		return true;
	}

	PROPERTY_TOSTRING_FUNCSIG(Int32) { return std::to_string(Value); }

	// UInt32
	PROPERTY_SERIALISE_FUNCSIG(UInt32)
	{
		jsonxx::Object obj;
		obj << "type" << "UInt32";
		obj << "value" << Value;
		json << _Name << obj;
		return true;
	}

	PROPERTY_DESERIALISE_FUNCSIG(UInt32)
	{
		Value = json.get<jsonxx::Number>("value", 0);
		return true;
	}

	PROPERTY_DESERIALISE_VAL_FUNCSIG(UInt32)
	{
		Value = json.get<jsonxx::Number>();
		return true;
	}

	PROPERTY_TOSTRING_FUNCSIG(UInt32) { return std::to_string(Value); }

	// Int64
	PROPERTY_SERIALISE_FUNCSIG(Int64)
	{
		jsonxx::Object obj;
		obj << "type" << "Int64";
		obj << "value" << Value;
		json << _Name << obj;
		return true;
	}

	PROPERTY_DESERIALISE_FUNCSIG(Int64)
	{
		Value = json.get<jsonxx::Number>("value", 0);
		return true;
	}
	
	PROPERTY_DESERIALISE_VAL_FUNCSIG(Int64)
	{
		Value = json.get<jsonxx::Number>();
		return true;
	}

	PROPERTY_TOSTRING_FUNCSIG(Int64) { return std::to_string(Value); }

	// UInt64
	PROPERTY_SERIALISE_FUNCSIG(UInt64)
	{
		jsonxx::Object obj;
		obj << "type" << "UInt64";
		obj << "value" << Value;
		json << _Name << obj;
		return true;
	}

	PROPERTY_DESERIALISE_FUNCSIG(UInt64)
	{
		Value = json.get<jsonxx::Number>("value", 0);
		return true;
	}

	PROPERTY_DESERIALISE_VAL_FUNCSIG(UInt64)
	{
		Value = json.get<jsonxx::Number>();
		return true;
	}

	PROPERTY_TOSTRING_FUNCSIG(UInt64) { return std::to_string(Value); }

	// Float
	PROPERTY_SERIALISE_FUNCSIG(Float)
	{
		jsonxx::Object obj;
		obj << "type" << "Float";
		obj << "value" << Value;
		json << _Name << obj;
		return true;
	}

	PROPERTY_DESERIALISE_FUNCSIG(Float)
	{
		Value = json.get<jsonxx::Number>("value", 0);
		return true;
	}

	PROPERTY_DESERIALISE_VAL_FUNCSIG(Float)
	{
		Value = json.get<jsonxx::Number>();
		return true;
	}

	PROPERTY_TOSTRING_FUNCSIG(Float) { return std::to_string(Value); }

	// Double
	PROPERTY_SERIALISE_FUNCSIG(Double)
	{
		jsonxx::Object obj;
		obj << "type" << "Double";
		obj << "value" << Value;
		json << _Name << obj;
		return true;
	}

	PROPERTY_DESERIALISE_FUNCSIG(Double)
	{
		Value = json.get<jsonxx::Number>("value", 0);
		return true;
	}

	PROPERTY_DESERIALISE_VAL_FUNCSIG(Double)
	{
		Value = json.get<jsonxx::Number>();
		return true;
	}

	PROPERTY_TOSTRING_FUNCSIG(Double) { return std::to_string(Value); }

	// String
	PROPERTY_SERIALISE_FUNCSIG(String)
	{
		jsonxx::Object obj;
		obj << "type" << "String";
		obj << "value" << Value;
		json << _Name << obj;
		return true;
	}

	PROPERTY_DESERIALISE_FUNCSIG(String)
	{
		Value = json.get<jsonxx::String>("value", "");
		return true;
	}

	PROPERTY_DESERIALISE_VAL_FUNCSIG(String)
	{
		Value = json.get<jsonxx::String>();
		return true;
	}

	PROPERTY_TOSTRING_FUNCSIG(String) { return Value; }

	// Vec2
	PROPERTY_SERIALISE_FUNCSIG(Vec2)
	{
		jsonxx::Object obj;
		obj << "type" << "Vec2";
		obj << "value" << Value;
		json << _Name << obj;
		return true;
	}

	PROPERTY_DESERIALISE_FUNCSIG(Vec2)
	{
		Value << json.get<jsonxx::Object>("value");
		return true;
	}

	PROPERTY_DESERIALISE_VAL_FUNCSIG(Vec2)
	{
		Value << json.get<jsonxx::Object>();
		return true;
	}

	PROPERTY_TOSTRING_FUNCSIG(Vec2) { return "[" + std::to_string(Value.x) + ", " + std::to_string(Value.y) + "]"; }

	// Vec3
	PROPERTY_SERIALISE_FUNCSIG(Vec3)
	{
		jsonxx::Object obj;
		obj << "type" << "Vec3";
		obj << "value" << Value;
		json << _Name << obj;
		return true;
	}

	PROPERTY_DESERIALISE_FUNCSIG(Vec3)
	{
		Value << json.get<jsonxx::Object>("value");
		return true;
	}
	
	PROPERTY_DESERIALISE_VAL_FUNCSIG(Vec3)
	{
		Value << json.get<jsonxx::Object>();
		return true;
	}

	PROPERTY_TOSTRING_FUNCSIG(Vec3) { return "[" + std::to_string(Value.x) + ", " + std::to_string(Value.y) + ", " + std::to_string(Value.z) + "]"; }

	// Boolean
	PROPERTY_SERIALISE_FUNCSIG(Bool)
	{
		jsonxx::Object obj;
		obj << "type" << "Bool";
		obj << "value" << Value;
		json << _Name << obj;
		return true;
	}

	PROPERTY_DESERIALISE_FUNCSIG(Bool)
	{
		Value = json.get<jsonxx::Boolean>("value", false);
		return true;
	}

	PROPERTY_DESERIALISE_VAL_FUNCSIG(Bool)
	{
		Value = json.get<jsonxx::Number>();
		return true;
	}

	PROPERTY_TOSTRING_FUNCSIG(Bool) { return std::to_string(Value); }

	// Color
	PROPERTY_SERIALISE_FUNCSIG(Color)
	{
		jsonxx::Object obj;
		obj << "type" << "Color";
		obj << "value" << Value;
		json << _Name << obj;
		return true;
	}

	PROPERTY_DESERIALISE_FUNCSIG(Color)
	{
		Value << json.get<jsonxx::Object>("value");
		return true;
	}

	PROPERTY_DESERIALISE_VAL_FUNCSIG(Color)
	{
		Value << json.get<jsonxx::Object>();
		return true;
	}

	PROPERTY_TOSTRING_FUNCSIG(Color) { return "[" + std::to_string(Value.r) + ", " + std::to_string(Value.g) + ", " + std::to_string(Value.b) + ", " + std::to_string(Value.a) + "]"; }

	// Texture
	PROPERTY_SERIALISE_FUNCSIG(Texture)
	{
		jsonxx::Object obj;
		obj << "type" << "Texture";
		if (Value != nullptr)
		{
			obj << "value" << &Value;
		}
		else
		{
			obj << "value" << RES_TEXTURE_DEFAULT;
		}
		json << _Name << obj;
		return true;
	}

	PROPERTY_DESERIALISE_FUNCSIG(Texture)
	{
		if (json.has<jsonxx::String>("value"))
		{
			Value = Application::Get().GetResources().Textures[UID(json.get<jsonxx::String>("value"))].get();
		}
		else
		{
			Value = Application::Get().GetResources().Textures[RES_TEXTURE_DEFAULT].get();
		}

		return true;
	}

	PROPERTY_DESERIALISE_VAL_FUNCSIG(Texture)
	{
		if (json.is<jsonxx::String>())
		{
			Value = Application::Get().GetResources().Textures[UID(json.get<jsonxx::String>())].get();
		}
		else
		{
			Value = Application::Get().GetResources().Textures[RES_TEXTURE_DEFAULT].get();
		}
		return true;
	}

	PROPERTY_TOSTRING_FUNCSIG(Texture) { return Value->GetUID().str().c_str(); }

	// Entity
	PROPERTY_SERIALISE_FUNCSIG(Entity)
	{
		jsonxx::Object obj;
		obj << "type" << "Entity";
		obj << "value" << Value;
		json << _Name << obj;
		return true;
	}

	PROPERTY_DESERIALISE_FUNCSIG(Entity)
	{
		Value = json.get<jsonxx::Number>("value", ENTITY_INVALID);
		return true;
	}

	PROPERTY_DESERIALISE_VAL_FUNCSIG(Entity)
	{
		Value = json.get<jsonxx::Number>();
		return true;
	}

	PROPERTY_TOSTRING_FUNCSIG(Entity) { return std::to_string(Value); }
}