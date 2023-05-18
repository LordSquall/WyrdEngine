/* local includes */
#include "FrameworkLayer.h"

/* core includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <properties/BaseProp.h>
#include <serial/PropSerialisers.h>


namespace WyrdHeadless
{

	FrameworkLayer::FrameworkLayer()
		: Layer("FrameworkLayer")
	{
	}

	FrameworkLayer::~FrameworkLayer()
	{

	}

	bool FrameworkLayer::OnAttach()
	{
		static std::string path = "./proptests.json";

		jsonxx::Object rootObject;
		jsonxx::Object testObjectOut;

		BasePropRef intProp = PropFactory::CreateProp("Int", "int_prop");
		intProp->Set<int>(100);
		intProp->Serialise(testObjectOut);

		BasePropRef int32Prop = PropFactory::CreateProp("Int32", "int32_prop");
		int32Prop->Set<int32_t>(200);
		int32Prop->Serialise(testObjectOut);

		BasePropRef uint32Prop = PropFactory::CreateProp("UInt32", "uint32_prop");
		uint32Prop->Set<uint32_t>(300);
		uint32Prop->Serialise(testObjectOut);
		
		BasePropRef int64Prop = PropFactory::CreateProp("Int64", "int64_prop");
		int64Prop->Set<int64_t>(400);
		int64Prop->Serialise(testObjectOut);
		
		BasePropRef uint64Prop = PropFactory::CreateProp("UInt64", "uint64_prop");
		uint64Prop->Set<uint64_t>(500);
		uint64Prop->Serialise(testObjectOut);

		BasePropRef stringProp = PropFactory::CreateProp("String", "string_prop");
		stringProp->Set<std::string>("Hello World!");
		stringProp->Serialise(testObjectOut);

		BasePropRef vec2Prop = PropFactory::CreateProp("Vec2", "vec2_prop");
		vec2Prop->Set<Vector2>({ 10.0f, 20.0f });
		vec2Prop->Serialise(testObjectOut);

		BasePropRef vec3Prop = PropFactory::CreateProp("Vec3", "vec3_prop");
		vec3Prop->Set<Vector3>({ 10.0f, 20.0f, 30.0f });
		vec3Prop->Serialise(testObjectOut);

		BasePropRef colorProp = PropFactory::CreateProp("Color", "color_prop");
		colorProp->Set<Color>({ 0.2f, 0.4f, 0.6f, 0.8f });
		colorProp->Serialise(testObjectOut);


		rootObject << "properties" << testObjectOut;
		std::ofstream out(path);
		out << rootObject.json();
		out.close();
		
		jsonxx::Object testObjectIn;
		std::ifstream f(path);
		if (f.is_open() == true)
		{
			std::ostringstream ss;
			ss << f.rdbuf();
		
			if (testObjectIn.parse(ss.str()) == true)
			{
				WYRD_INFO("Test Cases:");

				if (testObjectIn.has<jsonxx::Object>("properties"))
				{
					jsonxx::Object propObject = testObjectIn.get<jsonxx::Object>("properties");

					BasePropRef intProp = PropFactory::CreateProp("int_prop", propObject);
					WYRD_INFO("  intProp = {0}", intProp->Get<int>());

					BasePropRef int32Prop = PropFactory::CreateProp("int32_prop", propObject);
					WYRD_INFO("  int32Prop = {0}", int32Prop->Get<int32_t>());

					BasePropRef uint32Prop = PropFactory::CreateProp("uint32_prop", propObject);
					WYRD_INFO("  uint32Prop = {0}", uint32Prop->Get<uint32_t>());

					BasePropRef int64Prop = PropFactory::CreateProp("int64_prop", propObject);
					WYRD_INFO("  int64Prop = {0}", int64Prop->Get<int64_t>());

					BasePropRef uint64Prop = PropFactory::CreateProp("uint64_prop", propObject);
					WYRD_INFO("  uint64Prop = {0}", uint64Prop->Get<uint64_t>());

					BasePropRef vec2Prop = PropFactory::CreateProp("vec2_prop", propObject);
					WYRD_INFO("  vec2Prop = [{0},{1}]", vec2Prop->Get<Vector2>().x, vec2Prop->Get<Vector2>().y);

					BasePropRef vec3Prop = PropFactory::CreateProp("vec3_prop", propObject);
					WYRD_INFO("  vec3Prop = [{0},{1},{2}]", vec3Prop->Get<Vector3>().x, vec3Prop->Get<Vector3>().y, vec3Prop->Get<Vector3>().z);

					BasePropRef colorProp = PropFactory::CreateProp("color_prop", propObject);
					WYRD_INFO("  colorProp = [{0},{1},{2},{3}]", colorProp->Get<Color>().r, colorProp->Get<Color>().g, colorProp->Get<Color>().b, colorProp->Get<Color>().a);
				}
				else
				{
					WYRD_ERROR("missing 'properties' object}");
				}
			}
		}
		{
			jsonxx::Object testObjectIn;
			std::ifstream f(path);
			if (f.is_open() == true)
			{
				std::ostringstream ss;
				ss << f.rdbuf();
		
				if (testObjectIn.parse(ss.str()) == true)
				{
					WYRD_INFO("Test Cases:");
		
					if (testObjectIn.has<jsonxx::Object>("properties"))
					{
						jsonxx::Object propsObject = testObjectIn.get<jsonxx::Object>("properties");
		
						BasePropMapRef propMap = PropFactory::CreatePropList(propsObject);
						for (auto& [k, prop] : *propMap)
						{
							WYRD_INFO("  Prop[{0}] = {1}", prop->GetName(), prop->ToString());// , intProp->Get<int>());
						}
					}
					else
					{
						WYRD_ERROR("missing 'properties' object}");
					}
				}
			}
		}
		
		return true;
	}

	void FrameworkLayer::OnDetach()
	{

	}

	void FrameworkLayer::OnUpdate(Timestep ts)
	{

	}

	void FrameworkLayer::OnRender(Timestep ts, Renderer& renderer)
	{

	}

	void FrameworkLayer::OnEvent(Event& event)
	{
	}
}