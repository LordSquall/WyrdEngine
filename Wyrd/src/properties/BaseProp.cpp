/* Local includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "properties/BaseProp.h"

namespace Wyrd
{
    PROPERTY_FACTORY_REGISTER(Int, "Int", "System.Int", int);
    PROPERTY_FACTORY_REGISTER(Int32, "Int32", "System.Int32", int32_t);
    PROPERTY_FACTORY_REGISTER(UInt32, "UInt32", "System.UInt32", uint32_t);
    PROPERTY_FACTORY_REGISTER(Int64, "Int64", "System.Int64", int64_t);
    PROPERTY_FACTORY_REGISTER(UInt64, "UInt64", "System.UInt64", uint64_t);
    PROPERTY_FACTORY_REGISTER(String, "String", "System.String", std::string);
    PROPERTY_FACTORY_REGISTER(Float, "Float", "System.Single", float);
    PROPERTY_FACTORY_REGISTER(Double, "Double", "System.Double", double);
    PROPERTY_FACTORY_REGISTER(Vec2, "Vec2", "WyrdAPI.Vector2", Wyrd::Vector2);
    PROPERTY_FACTORY_REGISTER(Vec3, "Vec3", "WyrdAPI.Vector3", Wyrd::Vector3);
    PROPERTY_FACTORY_REGISTER(Bool, "Bool", "System.Boolean", bool);
    PROPERTY_FACTORY_REGISTER(Color, "Color", "WyrdAPI.Color", Wyrd::Color);
    PROPERTY_FACTORY_REGISTER(Texture, "Texture", "WyrdAPI.Texture", Wyrd::Texture*);
    PROPERTY_FACTORY_REGISTER(Entity, "Entity", "WyrdAPI.Entity", Wyrd::Entity);

    std::map<std::string, PropFactory::CreatePropFunc>* PropFactory::GetNativeProps()
    {
        static std::map<std::string, PropFactory::CreatePropFunc> nativeProps  {
        };

        return &nativeProps;
    }

    std::map<std::string, PropFactory::CreatePropFunc>* PropFactory::GetManagedProps()
    {
        static std::map<std::string, PropFactory::CreatePropFunc> managedProps{
        };

        return &managedProps;
    }
    
    bool PropFactory::Register(const std::string typeName,
        const std::string managedName,
        CreatePropFunc funcCreate)
    {
        auto nativeProperties = GetNativeProps();
        if (auto it = nativeProperties->find(typeName); it == nativeProperties->end())
        {
            (*nativeProperties)[typeName] = funcCreate;
        }

        auto managedProperties = GetManagedProps();
        if (auto it = managedProperties->find(managedName); it == managedProperties->end())
        {
            (*managedProperties)[managedName] = funcCreate;
        }
        return true;
    }

    BasePropRef PropFactory::CreateProp(const std::string& type,
        const std::string& name)
    {
        auto properties = GetNativeProps();
        if (auto it = properties->find(type); it != properties->end())
        {
            return (*properties)[type](name, type, 0, false);
        }
        return nullptr;
    }

    BasePropRef PropFactory::CreateProp(const std::string& name,
        const jsonxx::Object& object)
    {
        if (object.has<jsonxx::Object>(name))
        {
            jsonxx::Object innerObj = object.get<jsonxx::Object>(name);
            if (innerObj.has<jsonxx::String>("type"))
            {
                const std::string t = innerObj.get<jsonxx::String>("type");
                const jsonxx::Value v = innerObj.get<jsonxx::Value>("value");

                auto properties = GetNativeProps();
                if (auto it = properties->find(t); it != properties->end())
                {
                    return (*properties)[t](name, t, v, true);
                }
                else
                {
                    return nullptr;
                }
            }
            else
            {
                return nullptr;
            }
        }
        else
        {
            return nullptr;
        }
    }


    BasePropRef PropFactory::CreateManagedProp(const std::string& managedPropName, const std::string& name)
    {
        auto managedProperties = GetManagedProps();
        if (auto it = managedProperties->find(managedPropName); it != managedProperties->end())
        {
            return (*managedProperties)[managedPropName](name, managedPropName, 0, false);
        }
        return nullptr;
    }

    BasePropMapRef PropFactory::CreatePropMap(const jsonxx::Object& object)
    {
        BasePropMapRef propMap = std::make_unique<std::map<std::string, BasePropRef>>();

        for (auto [name, v] : object.kv_map())
        {
            if (v->is<jsonxx::Object>())
            {
                jsonxx::Object innerObj = v->get<jsonxx::Object>();

                const std::string t = innerObj.get<jsonxx::String>("type");
                const jsonxx::Value v = innerObj.get<jsonxx::Value>("value");
                
                auto properties = GetNativeProps();
                if (auto it = properties->find(t); it != properties->end())
                {
                    BasePropRef baseProp = (*properties)[t](name, t, v, true);
                    (*propMap)[name] = std::move(baseProp);
                }
            }
        }

        return propMap;
    }

    BasePropMapRef PropFactory::CopyPropMap(BasePropMapRef map)
    {
        BasePropMapRef propMap = std::make_shared<std::map<std::string, BasePropRef>>();
        for (auto& p : *map)
        {
            (*propMap)[p.first] = std::move(p.second->CreateCopy(p.second.get()));
        }

        return propMap;
    }
}