/* Local includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "properties/BaseProp.h"

namespace Wyrd
{
    PROPERTY_FACTORY_REGISTER(Int, "Int");
    PROPERTY_FACTORY_REGISTER(Int32, "Int32");
    PROPERTY_FACTORY_REGISTER(UInt32, "UInt32");
    PROPERTY_FACTORY_REGISTER(Int64, "Int64");
    PROPERTY_FACTORY_REGISTER(UInt64, "UInt64");
    PROPERTY_FACTORY_REGISTER(String, "String");
    PROPERTY_FACTORY_REGISTER(Vec2, "Vec2");
    PROPERTY_FACTORY_REGISTER(Vec3, "Vec3");
    PROPERTY_FACTORY_REGISTER(Color, "Color");
    PROPERTY_FACTORY_REGISTER(Texture, "Texture");

    std::map<std::string, PropFactory::CreatePropFunc>* PropFactory::GetProps()
    {
        static std::map<std::string, PropFactory::CreatePropFunc> properties  {
        };

        return &properties;
    }
    
    bool PropFactory::Register(const std::string typeName,
        CreatePropFunc funcCreate)
    {
        auto properties = GetProps();
        if (auto it = properties->find(typeName); it == properties->end())
        {
            (*properties)[typeName] = funcCreate;
            return true;
        }
        return false;
    }

    BasePropRef PropFactory::CreateProp(const std::string& type,
        const std::string& name)
    {
        auto properties = GetProps();
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

                auto properties = GetProps();
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
                
                auto properties = GetProps();
                if (auto it = properties->find(t); it != properties->end())
                {
                    BasePropRef baseProp = (*properties)[t](name, t, v, true);
                    (*propMap)[name] = std::move(baseProp);
                }
            }
        }

        return propMap;
    }
}