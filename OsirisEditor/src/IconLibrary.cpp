#include "osreditorpch.h"

#include "IconLibrary.h"

#include "core/Application.h"

#include <nlohmann/json.hpp>
#include <SOIL.h>

using namespace nlohmann;

namespace Osiris
{
	IconLibrary::IconLibrary()
	{
	}

	IconLibrary::~IconLibrary()
	{

	}


	bool IconLibrary::AddIconsFromFile(std::string& filepath)
	{
		int width, height, channels;

		/* load the json file */
		nlohmann::json;
		std::ifstream i(filepath);
		json j;
		i >> j;

		std::string relativePath = filepath.substr(0, filepath.find_last_of('/')) + '/';
		
		unsigned char* pixels = SOIL_load_image((relativePath + j["filename"].get<std::string>()).c_str(), &width, &height, &channels, false);
		
		unsigned int id = Application::Get().GetRenderer().LoadRawTexture(pixels, width, height, channels);
	
		for (auto& el : j["icons"].items()) {

			Icon icon = Icon();
			icon.SetHandle(id);
			icon.SetData(pixels);
			icon.SetXPos(el.value()["pos-x"]);
			icon.SetYPos(el.value()["pos-y"]);
			icon.SetWidth(el.value()["width"]);
			icon.SetHeight(el.value()["height"]);

			float uv0x = (icon.GetXPos() == 0) ? 0.0f : 1.0f / (width / icon.GetXPos());
			float uv0y = (icon.GetYPos() == 0) ? 0.0f : 1.0f / (height / icon.GetYPos());

			float uv1x = uv0x + 1.0f / (width / icon.GetWidth());
			float uv1y = uv0y + 1.0f / (height / icon.GetHeight());
			
			icon.SetUV0(uv0x, uv0y);
			icon.SetUV1(uv1x, uv1y);

			OSR_CORE_TRACE("Added Icon [{0}] - X:{1} Y:{2} W:{3} H:{4} UV0:{5},{6} UV1:{7},{8}",
				el.value()["name"].get<std::string>(), icon.GetXPos(), icon.GetYPos(), icon.GetWidth(), icon.GetHeight(), icon.GetUV0()[0], icon.GetUV0()[1], icon.GetUV1()[0], icon.GetUV1()[1]);

			AddIcon(el.value()["name"].get<std::string>(), icon);
		}

		return true;
	}


	Icon& IconLibrary::GetIconFromFileExtension(const std::filesystem::path& path)
	{
		Icon icon;

		std::string extension = path.extension().string();

		if (extension.compare(".txt") == 0)
		{
			icon = m_Icons["text_file"];
		}
		else
		{
			icon = m_Icons["unknown_file"];
		}

		return icon;
	}
}