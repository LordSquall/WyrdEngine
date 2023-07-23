#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"
#include "jsonxx.h"
#include "core/ResourcesUIDs.h"
#include "core/ecs/ECS.h"
#include "core/renderer/Texture.h"


#define PROPERTY_FACTORY_REGISTER(Name, typeName, managedName, InternalStorageType)\
bool Prop##Name::s_Registered = PropFactory::Register(typeName, managedName,Prop##Name::CreateJson,Prop##Name::CreateSerial);\
template void BaseProp::Set<InternalStorageType>(InternalStorageType);\
template InternalStorageType BaseProp::Get<InternalStorageType>();


#define PROPERTY_SERIALISE_FUNCSIG(Name) bool Prop##Name::Serialise(jsonxx::Object& json)
#define PROPERTY_SERIALISE_STREAM_FUNCSIG(Name) bool Prop##Name::Serialise(std::ofstream& stream)
#define PROPERTY_DESERIALISE_FUNCSIG(Name) bool Prop##Name::Deserialise(const jsonxx::Object& json)
#define PROPERTY_DESERIALISE_STREAM_FUNCSIG(Name) bool Prop##Name::Deserialise(std::ifstream& stream)
#define PROPERTY_DESERIALISE_VAL_FUNCSIG(Name) bool Prop##Name::Deserialise(const jsonxx::Value& json)

#define PROPERTY_TOSTRING_FUNCSIG(Name) const std::string Prop##Name::ToString()

#define PROPERTY_TEMPLATE_SET_FUNC(Name, Type) \
template <>\
void BaseProp::Set(Type value){  ((Prop##Name*)this)->Value = value; }

#define PROPERTY_TEMPLATE_WRAPPER_SET_FUNC(Name, Type, GetFunc) \
template <>\
void BaseProp::Set(Type value){  ((Prop##Name*)this)->Value = value->##GetFunc; }

#define PROPERTY_TEMPLATE_GET_FUNC(Name, Type) \
template <>\
Type BaseProp::Get(){  return ((Prop##Name*)this)->Value; }

#define PROPERTY_TEMPLATE_WRAPPER_GET_FUNC(Name, Type, GetFunc) \
template <>\
Type BaseProp::Get(){  return ((Prop##Name*)this)->Value; }

#define PROPERTY_TYPE(Name, ManagedName, InternalStorageType, DefaultValue)\
template<>\
WYRD_LIBRARY_API void BaseProp::Set<InternalStorageType>(InternalStorageType v);\
template<>\
WYRD_LIBRARY_API InternalStorageType BaseProp::Get<InternalStorageType>();\
class WYRD_LIBRARY_API Prop##Name :  public BaseProp\
{\
public:\
	Prop##Name() : BaseProp("", "", #ManagedName), Value(DefaultValue) {}\
	Prop##Name(const std::string& name) : BaseProp(name, "", #ManagedName), Value(DefaultValue) {}\
	Prop##Name(const std::string& name, const std::string& type) : BaseProp(name, type, #ManagedName), Value(DefaultValue) {}\
	Prop##Name(const std::string& name, const std::string& type, InternalStorageType value) : BaseProp(name, type, #ManagedName), Value(value) { }\
	Prop##Name(const std::string& name, const std::string& type, const jsonxx::Value& value) : BaseProp(name, type, #ManagedName), Value(DefaultValue) { Deserialise(value); }\
	Prop##Name(const std::string& name, const std::string& type, std::ifstream& value) : BaseProp(name, type, #ManagedName), Value(DefaultValue) { Deserialise(value); }\
	\
	bool Serialise(jsonxx::Object& json);\
	bool Serialise(std::ofstream& stream);\
	bool Deserialise(std::ifstream& stream);\
	bool Deserialise(const jsonxx::Object& json);\
	bool Deserialise(const jsonxx::Value& json);\
	void* GetRawValuePtr() { return &Value; }\
	std::unique_ptr<BaseProp> CreateCopy(BaseProp* ref) override { return std::make_unique<Prop##Name>(ref->GetName(), ref->GetType(), ref->Get<InternalStorageType>()); }\
	const std::string ToString();\
	static BasePropRef CreateJson(const std::string& name, const std::string& type, const jsonxx::Value& value, bool parseValue) { \
		return parseValue ? std::make_unique<Prop##Name>(name, type, value) : std::make_unique<Prop##Name>(name, type); \
	}\
	static BasePropRef CreateSerial(const std::string& name, const std::string& type, std::ifstream& value, bool parseValue) { \
		return parseValue ? std::make_unique<Prop##Name>(name, type, value) : std::make_unique<Prop##Name>(name, type); \
	}\
public:\
	InternalStorageType Value;\
protected:\
	static bool s_Registered;\
};



const std::string emptryStr = "";

namespace Wyrd
{
	class BaseProp;

	typedef std::unique_ptr<BaseProp> BasePropRef;
	typedef std::shared_ptr<std::map<std::string, BasePropRef>> BasePropMapRef;

	class WYRD_LIBRARY_API BaseProp {
	public:
		BaseProp(const std::string& name, const std::string& type, const std::string& managedType) : _Name(name), _Type(type), _ManagedType(managedType){}

		virtual bool Serialise(jsonxx::Object& json) = 0;
		virtual bool Deserialise(const jsonxx::Object& json) = 0;

		virtual bool Serialise(std::ofstream& stream) = 0;
		virtual bool Deserialise(std::ifstream& stream) = 0;

		virtual std::unique_ptr<BaseProp> CreateCopy(BaseProp* ref) = 0;

		virtual const std::string ToString() = 0;

		template <class T>
		void Set(T value) { printf("Called default Set template function!"); };

		template <class T>
		T Get() { printf("Called default Get template function!"); return nullptr; };

		virtual void* GetRawValuePtr() = 0;

		inline const std::string GetName() const { return _Name; };
		inline void SetName(const std::string& name) { _Name = name; };

		inline const std::string GetType() const { return _Type; };
		inline const std::string GetManagedType() const { return _ManagedType; };


	protected:
		std::string _Type;
		std::string _ManagedType;
		std::string _Name;
	};

	class WYRD_LIBRARY_API PropFactory
	{
	public:
		using CreateJsonPropFunc = BasePropRef(*)(const std::string& name, const std::string& type, const jsonxx::Value& v, bool parseValue);
		using CreateSerialPropFunc = BasePropRef(*)(const std::string& name, const std::string& type, std::ifstream& v, bool parseValue);

	public:
		PropFactory() = delete;

		static bool Register(const std::string typeName, const std::string managedName, CreateJsonPropFunc createJsonFunc, CreateSerialPropFunc createSerialFunc);

		static BasePropRef CreateProp(const std::string& type, const std::string& name);

		static BasePropRef CreateProp(const std::string& name, const jsonxx::Object& object);

		static BasePropRef CreateManagedProp(const std::string& managedPropName, const std::string& name);

		static BasePropMapRef CreatePropMap(const jsonxx::Object& object);

		static BasePropMapRef CreatePropMap(std::ifstream& stream);

		static BasePropMapRef CopyPropMap(BasePropMapRef map);

		static std::map<std::string, PropFactory::CreateJsonPropFunc>* GetNativeProps();
		static std::map<std::string, PropFactory::CreateJsonPropFunc>* GetManagedProps();
		static std::map<std::string, PropFactory::CreateSerialPropFunc>* GetSerialProps();
	};

	// Type name, Managed Type name, internal storage type, default value
	PROPERTY_TYPE(Int, System.Int, int, 0);
	PROPERTY_TYPE(Int32, System.Int32, int32_t, 0);
	PROPERTY_TYPE(UInt32, System.UInt32, uint32_t, 0);
	PROPERTY_TYPE(Int64, System.Int64, int64_t, 0);
	PROPERTY_TYPE(UInt64, System.UInt64, uint64_t, 0);
	PROPERTY_TYPE(Float, System.Single, float, 0.0f);
	PROPERTY_TYPE(Double, System.Double, double, 00.00);
	PROPERTY_TYPE(String, System.String, std::string, "");
	PROPERTY_TYPE(Vec2, WyrdAPI.Vector2, Wyrd::Vector2, Vector2());
	PROPERTY_TYPE(Vec3, WyrdAPI.Vector3, Wyrd::Vector3, Vector3());
	PROPERTY_TYPE(Bool, System.Boolean, bool, false);
	PROPERTY_TYPE(Color, WyrdAPI.Color, Wyrd::Color, Color());
	PROPERTY_TYPE(Texture, WyrdAPI.Texture, Wyrd::Texture*, nullptr);
	PROPERTY_TYPE(Entity, WyrdAPI.Entity, Wyrd::Entity, ENTITY_INVALID);
}
