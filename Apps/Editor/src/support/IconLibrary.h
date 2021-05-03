#pragma once

/* core wyrd includes */
#include <core/export.h>
#include <core/Layer.h>

/* local includes */
#include "datamodels/resources/TextureRes.h"

/* external includes */
#include <glm/glm.hpp>

namespace Wyrd::Editor
{
	class IconSet;

	class Icon
	{
	public:
		Icon() : width(0), height(0) {}
		Icon(const Icon& obj);
		virtual ~Icon() {}

		std::string name;
		unsigned int width = 0;
		unsigned int height = 0;
		unsigned int x = 0;
		unsigned int y = 0;
		
		glm::vec2 uv[4];
		std::shared_ptr<IconSet> iconSet;
	};

	class IconSet
	{
	public:
		IconSet() : width(0), height(0) {}

		std::string name;

		std::string imagePath;
		int width = 0;
		int height = 0;

		std::vector<std::shared_ptr<Icon>> Icons;
		std::shared_ptr<TextureRes> Texture;
	};

	class IconLibrary
	{
	public:
		IconLibrary();
		~IconLibrary();

		bool AddIconsFromFile(std::string& filepath);

		std::shared_ptr<Icon> GetIcon(std::string setName, std::string name);
		
	private:
		std::map<std::string, std::shared_ptr<IconSet>> _IconSets;
		std::shared_ptr<Icon> _DefaultIcon;
	};
}