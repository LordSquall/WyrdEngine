#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"
#include "jsonxx.h"
#include "core/ResourcesUIDs.h"
#include "core/renderer/Texture.h"


#define PROPERTY_FACTORY_REGISTER(Name, typeName)\
bool Prop##Name::s_Registered = PropFactory::Register(\
typeName,\
Prop##Name::Create)


#define PROPERTY_SERIALISE_FUNCSIG(Name) bool Prop##Name::Serialise(jsonxx::Object& json)
#define PROPERTY_DESERIALISE_FUNCSIG(Name) bool Prop##Name::Deserialise(const jsonxx::Object& json)
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

#define PROPERTY_TYPE(Name, Type, DefaultValue)\
class Prop##Name :  public BaseProp\
{\
public:\
	Prop##Name() : BaseProp("", ""), Value(DefaultValue) {}\
	Prop##Name(const std::string& name) : BaseProp(name, ""), Value(DefaultValue) {}\
	Prop##Name(const std::string& name, const std::string& type) : BaseProp(name, type), Value(DefaultValue) {}\
	Prop##Name(const std::string& name, const std::string& type, Type value) : BaseProp(name, type), Value(value) { }\
	Prop##Name(const std::string& name, const std::string& type, const jsonxx::Value& value) : BaseProp(name, type), Value(DefaultValue) { Deserialise(value); }\
	\
	bool Serialise(jsonxx::Object& json);\
	bool Deserialise(const jsonxx::Object& json);\
	bool Deserialise(const jsonxx::Value& json);\
	const std::string ToString();\
	static BasePropRef Create(const std::string& name, const std::string& type, const jsonxx::Value& value, bool parseValue) { \
		return parseValue ? std::make_unique<Prop##Name>(name, type, value) : std::make_unique<Prop##Name>(name, type); \
	}\
public:\
	Type Value;\
protected:\
	static bool s_Registered;\
};\
template<>\
void BaseProp::Set<Type>(Type v);\
template<>\
Type BaseProp::Get<Type>();\

#define PROPERTY_WRAPPER_TYPE(Name, Wrapper, Type, DefaultValue)\
class Prop##Name :  public BaseProp\
{\
public:\
	Prop##Name() : BaseProp("", ""), Value(DefaultValue) {}\
	Prop##Name(const std::string& name) : BaseProp(name, ""), Value(DefaultValue) {}\
	Prop##Name(const std::string& name, const std::string& type) : BaseProp(name, type), Value(DefaultValue) {}\
	Prop##Name(const std::string& name, const std::string& type, Type value) : BaseProp(name, type), Value(value) { }\
	Prop##Name(const std::string& name, const std::string& type, const jsonxx::Value& value) : BaseProp(name, type), Value(DefaultValue) { Deserialise(value); }\
	\
	bool Serialise(jsonxx::Object& json);\
	bool Deserialise(const jsonxx::Object& json);\
	bool Deserialise(const jsonxx::Value& json);\
	const std::string ToString();\
	static BasePropRef Create(const std::string& name, const std::string& type, const jsonxx::Value& value, bool parseValue) { \
		return parseValue ? std::make_unique<Prop##Name>(name, type, value) : std::make_unique<Prop##Name>(name, type); \
	}\
public:\
	Type Value;\
protected:\
	static bool s_Registered;\
};\
template<>\
void BaseProp::Set<Wrapper>(Wrapper v);\
template<>\
Wrapper BaseProp::Get<Wrapper>();\


const std::string emptryStr = "";

namespace Wyrd
{
	class BaseProp;

	typedef std::unique_ptr<BaseProp> BasePropRef;
	typedef std::shared_ptr<std::map<std::string, BasePropRef>> BasePropMapRef;

	class BaseProp {
	public:
		BaseProp(const std::string& name, const std::string& type) : _Name(name), _Type(type) {}

		virtual bool Serialise(jsonxx::Object& json) = 0;
		virtual bool Deserialise(const jsonxx::Object& json) = 0;

		virtual const std::string ToString() = 0;

		template <class T>
		void Set(T value) { printf("Called default Set template function!"); };

		template <class T>
		T Get() { printf("Called default Get template function!"); return default(T); };

		inline const std::string GetName() const { return _Name; };
		inline void SetName(const std::string& name) { _Name = name; };

		inline const std::string GetType() const { return _Type; };


	protected:
		std::string _Type;
		std::string _Name;
	};

	class WYRD_LIBRARY_API PropFactory
	{
	public:
		using CreatePropFunc = BasePropRef(*)(const std::string& name, const std::string& type, const jsonxx::Value& v, bool parseValue);
	
	public:
		PropFactory() = delete;
	
		static bool Register(const std::string typeName, CreatePropFunc createFunc);

		static BasePropRef CreateProp(const std::string& type, const std::string& name);

		static BasePropRef CreateProp(const std::string& name, const jsonxx::Object& object);

		static BasePropMapRef CreatePropMap(const jsonxx::Object& object);
	
		static std::map<std::string, PropFactory::CreatePropFunc>* GetProps();
	};

	PROPERTY_TYPE(Int, int, 0);
	PROPERTY_TYPE(Int32, int32_t, 0);
	PROPERTY_TYPE(UInt32, uint32_t, 0);
	PROPERTY_TYPE(Int64, int64_t, 0);
	PROPERTY_TYPE(UInt64, uint64_t, 0);
	PROPERTY_TYPE(String, std::string, "");
	PROPERTY_TYPE(Vec2, Vector2, Vector2());
	PROPERTY_TYPE(Vec3, Vector3, Vector3());
	PROPERTY_TYPE(Color, Color, Color());
	PROPERTY_TYPE(Texture, Texture*, nullptr);
}