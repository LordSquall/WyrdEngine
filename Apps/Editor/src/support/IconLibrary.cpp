#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/Log.h>

/* local includes */
#include "IconLibrary.h"
#include "loaders/IconSetLoader.h"

/* external includes */
#include <SOIL.h>

namespace Osiris::Editor
{
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
		float defaultIconWidth = 16u;
		float defaultIconHeight = 16u;

		// setup a default icon set with icon to handle missing icons
		std::shared_ptr<IconSet> defaultIconSet = std::make_shared<IconSet>();
		defaultIconSet->name = "defaultIconSet";
		defaultIconSet->imagePath = Utils::GetEditorResFolder() + "icons\\default.png";
		defaultIconSet->height = defaultIconHeight;
		defaultIconSet->width = defaultIconWidth;
		defaultIconSet->Texture = std::make_shared<TextureRes>(Utils::GetEditorResFolder() + "icons\\default.png");

		_DefaultIcon = std::make_shared<Icon>();
		_DefaultIcon->height = defaultIconHeight;
		_DefaultIcon->width = defaultIconWidth;
		_DefaultIcon->x = 0u;
		_DefaultIcon->y = defaultIconHeight;
		_DefaultIcon->name = "defaultIcon";

		float widthNorm = (float)_DefaultIcon->width / (float)defaultIconSet->width;
		float heightNorm = (float)_DefaultIcon->height / (float)defaultIconSet->height;
		float xNorm = (float)_DefaultIcon->x / (float)defaultIconSet->width;
		float yNorm = 1.0f - (float)_DefaultIcon->y / (float)defaultIconSet->height;

		_DefaultIcon->uv[0] = { xNorm,				yNorm };
		_DefaultIcon->uv[1] = { xNorm,				yNorm + heightNorm };
		_DefaultIcon->uv[2] = { xNorm + widthNorm,	yNorm + heightNorm };
		_DefaultIcon->uv[3] = { xNorm + widthNorm,	yNorm };


		_DefaultIcon->iconSet = defaultIconSet;

		defaultIconSet->Icons.push_back(_DefaultIcon);
	}

	IconLibrary::~IconLibrary()
	{

	}


	bool IconLibrary::AddIconsFromFile(std::string& filepath)
	{
		std::shared_ptr<IconSet> iconSet = std::make_shared<IconSet>();

		IconSetLoader::Result result = IconSetLoader::Load(filepath, iconSet);
		if (result == IconSetLoader::Result::Success)
		{			
			_IconSets[iconSet->name] = iconSet;
		}
		else
		{
			//TODO
		}

		return false;
	}

	std::shared_ptr<Icon> IconLibrary::GetIcon(std::string setName, std::string name)
	{
		auto setIt = _IconSets.find(setName);
		if (setIt != _IconSets.end())
		{
			auto it = std::find_if(_IconSets[setName]->Icons.begin(), _IconSets[setName]->Icons.end(), [&name](const std::shared_ptr<Icon>& obj) {return (*obj).name == name; });
			if (it != _IconSets[setName]->Icons.end())
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
}