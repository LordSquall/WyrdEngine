#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>

/* local includes */
#include "ObjLoader.h"

/* external include */

namespace Wyrd::Editor
{
	ObjLoader::Result ObjLoader::Load(std::string path, std::vector<Vertex2D>& vertices, std::vector<uint32_t>& indices, std::map<std::string, std::pair<uint32_t, uint32_t>>& vertexGroups, float scaleFactor)
	{
		ObjLoader::Result result = Success;

		std::ifstream filestream(path);

		std::string line;
		std::string currentGroupName = "";
		while (std::getline(filestream, line))
		{
			/* Groups */
			if (line[0] == 'g')
			{
				if (currentGroupName != "")
				{
					vertexGroups[currentGroupName].second = vertices.size() - 1;
				}

				char tag;
				std::istringstream iss(line);
				iss >> tag >> currentGroupName;

				// condition the name to remove any unwanted suffix
				if (currentGroupName.find_last_of('_') != std::string::npos)
				{
					currentGroupName = currentGroupName.substr(0, currentGroupName.find_last_of('_'));
				}

				vertexGroups[currentGroupName] = { vertices.size(), 0 };
			}

			/* Vertices */
			if (line[0] == 'v')
			{
				char tag;
				float x, y, z;
				std::istringstream iss(line);
				iss >> tag >> x >> y >> z;

				x *= scaleFactor;
				y *= scaleFactor; 
				z *= scaleFactor;

				vertices.push_back({ x, z, 1.0f, 1.0f, 1.0f, 1.0f });
			}

			/* Indices */
			if (line[0] == 'f')
			{
				char tag;
				uint32_t p1, p2, p3;
				std::istringstream iss(line);
				iss >> tag >> p1 >> p2 >> p3;
				indices.push_back(p1 - 1);
				indices.push_back(p2 - 1);
				indices.push_back(p3 - 1);
			}
		}
		vertexGroups[currentGroupName].second = vertices.size() - 1;

		// DEBUG
		//for (auto& v : vertices)
		//{
		//	WYRD_TRACE("V: x:{0} y:{1} r:{2} g:{3} b:{4} a:{5}", v.x, v.y, v.r, v.g, v.b, v.a);
		//}

		//for (int i = 0; i < indices.size(); i = i+3)
		//{
		//	WYRD_TRACE("I: p0:{0} p1:{1} p2:{2}", indices[i], indices[i+1], indices[i+2]);
		//}

		//for (auto& g : vertexGroups)
		//{
		//	WYRD_TRACE("G: name:{0} s:{1} e:{2}", g.first, g.second.first, g.second.second);
		//}

		return result;
	}
}