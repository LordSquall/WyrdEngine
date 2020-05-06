#pragma once

#include "osrpch.h"

#include "IconLibrary.h"

#include <nlohmann/json.hpp>
#include <SOIL.h>


using namespace nlohmann;

namespace Osiris::Editor
{
	/* Json operation overloader functions */
	void to_json(json& jIconSet, const IconSet& iconSet);
	void from_json(const json& jIconSet, IconSet& iconSet);
	void to_json(json& jIcon, const Icon& icon);
	void from_json(const json& jIcon, Icon& icon);

	Icon::Icon(const Icon& obj)
	{
		name = obj.name;
		width = obj.width;
		height = obj.height;
		x = obj.x;
		y = obj.y;
	}

	IconLibrary::IconLibrary()
	{

	}

	IconLibrary::~IconLibrary()
	{

	}


	bool IconLibrary::AddIconsFromFile(std::string& filepath)
	{
		IconSet iconSet;

		IconSetLoader::Result result = IconSetLoader::Load(filepath, iconSet);
		if (result == IconSetLoader::Result::Success)
		{			
			_IconSets[iconSet.name] = iconSet;
		}
		else
		{
			//TODO
		}

		return false;
	}

	std::shared_ptr<Icon> IconLibrary::GetIcon(std::string& setName, std::string& name)
	{
		auto setIt = _IconSets.find(setName);
		if (setIt != _IconSets.end())
		{
			auto it = std::find_if(_IconSets[setName].Icons.begin(), _IconSets[setName].Icons.end(), [&name](const std::shared_ptr<Icon>& obj) {return (*obj).name == name; });
			if (it != _IconSets[setName].Icons.end())
			{
				return *it;
			}
			else
			{
				OSR_WARN("Unable to find matching icon {0} in iconset {0}", name, setName);
				return _DefaultIcon;
			}
		}
		else
		{
			OSR_WARN("Unable to find icon set {0}", setName);
			return _DefaultIcon;
		}
	}

	IconSetLoader::Result IconSetLoader::Load(std::string path, IconSet& iconSet)
	{
		json j;
		std::ifstream i(path);

		if (i.is_open())
		{
			i >> j;
			iconSet = j.get<IconSet>();

			/* create and load the icon set texture */
			iconSet.Texture = std::make_shared<TextureRes>(Utils::GetPath(path) + "/" + iconSet.imagePath);

			/* iterate the icons calculate all the offset and build iconset links */
			for each (auto& icon in iconSet.Icons)
			{
				float widthNorm = (float)icon->width / (float)iconSet.width;
				float heightNorm = (float)icon->height / (float)iconSet.height;
				float xNorm = (float)icon->x / (float)iconSet.width;
				float yNorm = 1.0f - (float)icon->y / (float)iconSet.height;

				icon->uv[0] = { xNorm,				yNorm };
				icon->uv[1] = { xNorm,				yNorm + heightNorm };
				icon->uv[2] = { xNorm + widthNorm,	yNorm + heightNorm };
				icon->uv[3] = { xNorm + widthNorm,	yNorm };

				icon->iconSet = std::make_shared<IconSet>(iconSet);
			}
			return Success;
		}
		return FileNotFound;
	}

	void to_json(json& jIconSet, const IconSet& iconSet) {
		jIconSet = json::object();

		jIconSet["name"] = iconSet.name;
		jIconSet["texture-path"] = iconSet.imagePath;
		jIconSet["texture-width"] = iconSet.width;
		jIconSet["texture-height"] = iconSet.height;
		jIconSet["icons"] = iconSet.Icons;
	}

	void from_json(const json& jIconSet, IconSet& iconSet) {
		jIconSet.at("name").get_to(iconSet.name);
		jIconSet.at("texture-path").get_to(iconSet.imagePath);
		jIconSet.at("texture-width").get_to(iconSet.width);
		jIconSet.at("texture-height").get_to(iconSet.height);
		jIconSet.at("icons").get_to(iconSet.Icons);
	}

	void to_json(json& jIcon, const Icon& icon) {
		jIcon = json::object();

		jIcon["name"] = icon.name;
		jIcon["width"] = icon.width;
		jIcon["height"] = icon.height;
		jIcon["x"] = icon.x;
		jIcon["y"] = icon.y;
	}

	void from_json(const json& jIcon, Icon& icon) {
		jIcon.at("name").get_to(icon.name);
		jIcon.at("width").get_to(icon.width);
		jIcon.at("height").get_to(icon.height);
		jIcon.at("x").get_to(icon.x);
		jIcon.at("y").get_to(icon.y);
	}
}