#pragma once

/* core osiris includes */
#include <osrpch.h>

/* local includes */
#include "IconSetLoader.h"
#include "support/IconLibrary.h"

/* external include */
#include <jsonxx.h>

namespace Osiris::Editor
{
	IconSetLoader::Result IconSetLoader::Load(std::string path, std::shared_ptr<IconSet> iconSet)
	{
		IconSetLoader::Result result = Success;
		jsonxx::Object o;
		
		std::ifstream f(path);

		if (f.is_open() == true) {
			std::ostringstream ss;
			ss << f.rdbuf();
			
			if (o.parse(ss.str()) == true)
			{
				iconSet->name = o.get<jsonxx::String>("name");
				iconSet->imagePath = o.get<jsonxx::String>("texture-path");
				iconSet->width = (int)o.get<jsonxx::Number>("texture-width");
				iconSet->height = (int)o.get<jsonxx::Number>("texture-height");
				iconSet->Texture = std::make_shared<TextureRes>(Utils::GetPath(path) + "/" + iconSet->imagePath);

				jsonxx::Array icons = o.get<jsonxx::Array>("icons");
				for (int i = 0; i < icons.size(); i++)
				{
					jsonxx::Object iconJson = icons.get<jsonxx::Object>(i);

					std::shared_ptr<Icon> icon = std::make_shared<Icon>();

					icon->name = iconJson.get<jsonxx::String>("name");
					icon->x = (unsigned int)iconJson.get<jsonxx::Number>("x");
					icon->y = (unsigned int)iconJson.get<jsonxx::Number>("y");
					icon->width = (unsigned int)iconJson.get<jsonxx::Number>("width");
					icon->height = (unsigned int)iconJson.get<jsonxx::Number>("height");

					float widthNorm = (float)icon->width / (float)iconSet->width;
					float heightNorm = (float)icon->height / (float)iconSet->height;
					float xNorm = (float)icon->x / (float)iconSet->width;
					float yNorm = 1.0f - (float)icon->y / (float)iconSet->height;

					icon->uv[0] = { xNorm,				yNorm };
					icon->uv[1] = { xNorm,				yNorm + heightNorm };
					icon->uv[2] = { xNorm + widthNorm,	yNorm + heightNorm };
					icon->uv[3] = { xNorm + widthNorm,	yNorm };

					icon->iconSet = iconSet;

					iconSet->Icons.push_back(icon);
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
}