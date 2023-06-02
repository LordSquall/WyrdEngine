/* Local includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "core/behaviour/PropMonoFactory.h"
#include "core/behaviour/ScriptedClass.h"
#include "core/Application.h"
#include "properties/BaseProp.h"

#include "MonoUtils.h"

namespace Wyrd
{
    void SetPropMono_Int(BaseProp* prop, MonoImage* image, const std::string& className, MonoObject* object)
    {
        MonoUtils::SetProperty(image, "WyrdGame", className, prop->GetName(),object, { prop->GetRawValuePtr() });
    }

    void SetPropMono_Int32(BaseProp* prop, MonoImage* image, const std::string& className, MonoObject* object)
    {
        MonoUtils::SetProperty(image, "WyrdGame", className, prop->GetName(), object, { prop->GetRawValuePtr() });
    }

    void SetPropMono_UInt32(BaseProp* prop, MonoImage* image, const std::string& className, MonoObject* object)
    {
        MonoUtils::SetProperty(image, "WyrdGame", className, prop->GetName(), object, { prop->GetRawValuePtr() });
    }

    void SetPropMono_Int64(BaseProp* prop, MonoImage* image, const std::string& className, MonoObject* object)
    {
        MonoUtils::SetProperty(image, "WyrdGame", className, prop->GetName(), object, { prop->GetRawValuePtr() });
    }

    void SetPropMono_UInt64(BaseProp* prop, MonoImage* image, const std::string& className, MonoObject* object)
    {
        MonoUtils::SetProperty(image, "WyrdGame", className, prop->GetName(), object, { prop->GetRawValuePtr() });
    }

    void SetPropMono_Float(BaseProp* prop, MonoImage* image, const std::string& className, MonoObject* object)
    {
        MonoUtils::SetProperty(image, "WyrdGame", className, prop->GetName(), object, { prop->GetRawValuePtr() });
    }

    void SetPropMono_Double(BaseProp* prop, MonoImage* image, const std::string& className, MonoObject* object)
    {
        MonoUtils::SetProperty(image, "WyrdGame", className, prop->GetName(), object, { prop->GetRawValuePtr() });
    }

    void SetPropMono_String(BaseProp* prop, MonoImage* image, const std::string& className, MonoObject* object)
    {
        MonoUtils::SetProperty(image, "WyrdGame", className, prop->GetName(), object, { 
            mono_string_new((MonoDomain*)Application::Get().GetBehaviour().GetDomain(), prop->Get<std::string>().c_str()) 
            });
    }

    void SetPropMono_Vec2(BaseProp* prop, MonoImage* image, const std::string& className, MonoObject* object)
    {
        Vector2 vector2 = prop->Get<Vector2>();

        std::vector<void*> args;

        std::shared_ptr<ScriptedClass> vector2Class = Application::Get().GetBehaviour().GetClass("Vector2");

        MonoObject* vector2Object = mono_object_new((MonoDomain*)Application::Get().GetBehaviour().GetDomain(), *vector2Class->ManagedClass);

        MonoProperty* xProperty = mono_class_get_property_from_name((MonoClass*)*vector2Class->ManagedClass, "X");
        MonoProperty* yProperty = mono_class_get_property_from_name((MonoClass*)*vector2Class->ManagedClass, "Y");

        MonoMethod* xPropSetter = mono_property_get_set_method(xProperty);
        MonoMethod* yPropSetter = mono_property_get_set_method(yProperty);

        args.push_back(&vector2.x);
        args.push_back(&vector2.y);

        mono_runtime_invoke(xPropSetter, vector2Object, &args[0], nullptr);
        mono_runtime_invoke(yPropSetter, vector2Object, &args[1], nullptr);

        args.clear();

        args.push_back(vector2Object);

        MonoUtils::SetProperty(image, "WyrdGame", className, prop->GetName(), object, { args });
    }

    void SetPropMono_Vec3(BaseProp* prop, MonoImage* image, const std::string& className, MonoObject* object)
    {
        Vector3 vector3 = prop->Get<Vector3>();

        std::vector<void*> args;

        std::shared_ptr<ScriptedClass> vector3Class = Application::Get().GetBehaviour().GetClass("Vector3");
        MonoObject* vector3Object = mono_object_new((MonoDomain*)Application::Get().GetBehaviour().GetDomain(), *vector3Class->ManagedClass);

        MonoProperty* xProperty = mono_class_get_property_from_name((MonoClass*)*vector3Class->ManagedClass, "X");
        MonoProperty* yProperty = mono_class_get_property_from_name((MonoClass*)*vector3Class->ManagedClass, "Y");
        MonoProperty* zProperty = mono_class_get_property_from_name((MonoClass*)*vector3Class->ManagedClass, "Z");

        MonoMethod* xPropSetter = mono_property_get_set_method(xProperty);
        MonoMethod* yPropSetter = mono_property_get_set_method(yProperty);
        MonoMethod* zPropSetter = mono_property_get_set_method(zProperty);

        args.push_back(&vector3.x);
        args.push_back(&vector3.y);
        args.push_back(&vector3.z);

        mono_runtime_invoke(xPropSetter, vector3Object, &args[0], nullptr);
        mono_runtime_invoke(yPropSetter, vector3Object, &args[1], nullptr);
        mono_runtime_invoke(zPropSetter, vector3Object, &args[2], nullptr);

        args.clear();

        args.push_back(vector3Object);

        MonoUtils::SetProperty(image, "WyrdGame", className, prop->GetName(), object, { args });
    }

    void SetPropMono_Bool(BaseProp* prop, MonoImage* image, const std::string& className, MonoObject* object)
    {
        MonoUtils::SetProperty(image, "WyrdGame", className, prop->GetName(), object, { prop->GetRawValuePtr() });
    }

    void SetPropMono_Color(BaseProp* prop, MonoImage* image, const std::string& className, MonoObject* object)
    {
        /* cast data to color structure */
        Color color = prop->Get<Color>();
        std::vector<void*> args;

        std::shared_ptr<ScriptedClass> colorClass = Application::Get().GetBehaviour().GetClass("Color");
        MonoObject* colorObject = mono_object_new((MonoDomain*)Application::Get().GetBehaviour().GetDomain(), *colorClass->ManagedClass);

        /* Call the object default constructor */
        MonoProperty* rProperty = mono_class_get_property_from_name((MonoClass*)*colorClass->ManagedClass, "R");
        MonoProperty* gProperty = mono_class_get_property_from_name((MonoClass*)*colorClass->ManagedClass, "G");
        MonoProperty* bProperty = mono_class_get_property_from_name((MonoClass*)*colorClass->ManagedClass, "B");
        MonoProperty* aProperty = mono_class_get_property_from_name((MonoClass*)*colorClass->ManagedClass, "A");

        MonoMethod* rPropSetter = mono_property_get_set_method(rProperty);
        MonoMethod* gPropSetter = mono_property_get_set_method(gProperty);
        MonoMethod* bPropSetter = mono_property_get_set_method(bProperty);
        MonoMethod* aPropSetter = mono_property_get_set_method(aProperty);

        args.push_back(&color.r);
        args.push_back(&color.g);
        args.push_back(&color.b);
        args.push_back(&color.a);

        mono_runtime_invoke(rPropSetter, colorObject, &args[0], nullptr);
        mono_runtime_invoke(gPropSetter, colorObject, &args[1], nullptr);
        mono_runtime_invoke(bPropSetter, colorObject, &args[2], nullptr);
        mono_runtime_invoke(aPropSetter, colorObject, &args[3], nullptr);

        args.clear();

        args.push_back(colorObject);

        MonoUtils::SetProperty(image, "WyrdGame", className, prop->GetName(), object, { args });
    }

    void SetPropMono_Texture(BaseProp* prop, MonoImage* image, const std::string& className, MonoObject* object)
    {
        Texture* texture = prop->Get<Texture*>();
        std::vector<void*> args;

        std::shared_ptr<ScriptedClass> textureClass = Application::Get().GetBehaviour().GetClass("Texture");
        MonoObject* managedTextureObject = Application::Get().GetBehaviour().RetrieveCachedManagedTextureObject(texture->GetUID());

        if (managedTextureObject)
        {
            args.push_back(managedTextureObject);
            MonoUtils::SetProperty(image, "WyrdGame", className, prop->GetName(), object, { args });
        }
        else
        {
            WYRD_CORE_WARN("Unable to Set texture prop!");
        }
        return;
    }

    void SetPropMono_Entity(BaseProp* prop, MonoImage* image, const std::string& className, MonoObject* object)
    {
        /* cast data to color structure */
        Entity entity = prop->Get<Entity>();
        std::vector<void*> args;

        std::shared_ptr<ScriptedClass> entityClass = Application::Get().GetBehaviour().GetClass("Entity");
        MonoObject* entityObject = mono_object_new((MonoDomain*)Application::Get().GetBehaviour().GetDomain(), *entityClass->ManagedClass);

        /* Call the object default constructor */
        MonoProperty* nativeProperty = mono_class_get_property_from_name((MonoClass*)*entityClass->ManagedClass, "NativeID");

        MonoMethod* nativePropSetter = mono_property_get_set_method(nativeProperty);

        args.push_back(&entity);

        mono_runtime_invoke(nativePropSetter, entityObject, &args[0], nullptr);

        args.clear();

        args.push_back(entityObject);

        MonoUtils::SetProperty(image, "WyrdGame", className, prop->GetName(), object, { args });
        return;
    }

    std::map<std::string, PropMonoFactory::SetPropMonoFunc>* PropMonoFactory::GetProps()
    {
        static std::map<std::string, PropMonoFactory::SetPropMonoFunc> properties{
            { "Int", SetPropMono_Int },
            { "Int32", SetPropMono_Int32 },
            { "UInt32", SetPropMono_UInt32 },
            { "Int64", SetPropMono_Int64 },
            { "UInt64", SetPropMono_UInt64 },
            { "Float", SetPropMono_Float },
            { "Double", SetPropMono_Double },
            { "String", SetPropMono_String },
            { "Vec2", SetPropMono_Vec2 },
            { "Vec3", SetPropMono_Vec3 },
            { "Bool", SetPropMono_Bool },
            { "Color", SetPropMono_Color },
            { "Texture", SetPropMono_Texture },
            { "Entity", SetPropMono_Entity }
        };
        return &properties;
    }

    bool PropMonoFactory::Register(const std::string typeName, SetPropMonoFunc setFunc)
    {
        auto properties = GetProps();
        if (auto it = properties->find(typeName); it == properties->end())
        {
            (*properties)[typeName] = setFunc;
            return true;
        }
        return false;
    }

    void PropMonoFactory::SetProperty(BaseProp* prop, MonoImage* image, const std::string& className, MonoObject* object)
    {
        auto properties = GetProps();
        if (properties->find(prop->GetType()) != properties->end())
        {
            properties->at(prop->GetType())(prop, image, className, object);
        }
        else
        {
            WYRD_CORE_ERROR("Missing SetPropMono Function for type {0}", prop->GetType());
        }
    }
}
