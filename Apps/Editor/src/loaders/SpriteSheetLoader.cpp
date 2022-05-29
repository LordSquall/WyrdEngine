#pragma once

/* core wyrd includes */
#include <core/Log.h>
#include <serial/TypeSerialisers.h>

/* local includes */
#include "loaders/SpriteSheetLoader.h"

/* external includes */
#include <jsonxx.h>

namespace Wyrd::Editor
{
	SpriteSheetLoader::Result SpriteSheetLoader::Load(const std::filesystem::path& path, std::vector<ImageArea>& imageAreas)
	{
		SpriteSheetLoader::Result result = Success;
		jsonxx::Object o;
				
		std::ifstream f(path);
		
		if (f.is_open() == true) {
			std::ostringstream ss;
			ss << f.rdbuf();
		
			if (o.parse(ss.str()) == true)
			{
				
				jsonxx::Array subImageAreas = o.get<jsonxx::Array>("subImageAreas");
		
				for (int i = 0; i < subImageAreas.size(); i++)
				{
					jsonxx::Object subImageAreaObj = subImageAreas.get<jsonxx::Object>(i);

					int32_t id = subImageAreaObj.get<jsonxx::Number>("id");
					std::string name = subImageAreaObj.get<jsonxx::String>("name");

					Vector2 uv1;
					//uv1 << subImageAreaObj.get<jsonxx::Array>("uv1");

					Vector2 uv2;
					//uv2 << subImageAreaObj.get<jsonxx::Array>("uv2");

					imageAreas.push_back({ id, name,  uv1, uv2 });
				}
			}
			else
			{
				result = FileMalformed;
			}
		}
		else
		{
			result = FileNotFound;
		}

		return result;
	}

	SpriteSheetLoader::Result SpriteSheetLoader::Save(const std::filesystem::path& path, const std::vector<ImageArea>& imageAreas)
	{
		SpriteSheetLoader::Result result = Success;
		jsonxx::Object o;

		jsonxx::Array subImageAreas;
		for (auto& subImageArea : imageAreas)
		{
			jsonxx::Object subImageAreaObj;

			subImageAreaObj << "id" << subImageArea.id;
			subImageAreaObj << "name" << subImageArea.name;
			subImageAreaObj << "uv1" << subImageArea.uv1;
			subImageAreaObj << "uv2" << subImageArea.uv2;

			subImageAreas << subImageAreaObj;
		}

		o << "subImageAreas" << subImageAreas;

		std::ofstream out(path);
		out << o.json();
		out.close();

		return result;
	}
}
