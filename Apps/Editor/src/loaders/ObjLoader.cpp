/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>

/* local includes */
#include "ObjLoader.h"

/* external include */

namespace Wyrd::Editor
{
	ObjLoader::Result ObjLoader::Load(std::string path, std::vector<Vertex3D>* vertices, std::vector<uint32_t>* indices, std::map<std::string, std::pair<uint32_t, uint32_t>>* vertexGroups, float scaleFactor)
	{
		ObjLoader::Result result = Success;

		std::vector<Vector3> verts;
		std::vector<Vector2> uvCoords;

		std::ifstream filestream(path);

		std::string line;
		std::string currentGroupName = "";
		while (std::getline(filestream, line))
		{
			/* Vertices */
			if (line[0] == 'v' && line[1] == ' ')
			{
				char tag;
				float x, y, z;
				std::istringstream iss(line);
				iss >> tag >> x >> y >> z;

				x *= scaleFactor;
				y *= scaleFactor; 
				z *= scaleFactor;

				verts.push_back({ x, y, z });
			}

			/* UV Coords */
			if (line[0] == 'v' && line[1] == 't')
			{
				std::string tag;
				float u, v;
				std::istringstream iss(line);
				iss >> tag >> u >> v;

				uvCoords.push_back({ u, v });
			}

			/* faces */
			if (line[0] == 'f')
			{
				char tag;
				std::string set1, set2, set3;
				std::istringstream iss(line);
				iss >> tag >> set1 >> set2 >> set3;

				{
					int vertIdx, textureIdx;
					sscanf(set1.c_str(), "%d/%d", &vertIdx, &textureIdx);
					vertIdx--;
					textureIdx--;

					vertices->push_back({ verts[vertIdx].x, verts[vertIdx].y , verts[vertIdx].z, uvCoords[textureIdx].x, uvCoords[textureIdx].y });
				}
				{
					int vertIdx, textureIdx;
					sscanf(set2.c_str(), "%d/%d", &vertIdx, &textureIdx);
					vertIdx--;
					textureIdx--;

					vertices->push_back({ verts[vertIdx].x, verts[vertIdx].y , verts[vertIdx].z, uvCoords[textureIdx].x, uvCoords[textureIdx].y });
				}
				{
					int vertIdx, textureIdx;
					sscanf(set3.c_str(), "%d/%d", &vertIdx, &textureIdx);
					vertIdx--;
					textureIdx--;

					vertices->push_back({ verts[vertIdx].x, verts[vertIdx].y , verts[vertIdx].z, uvCoords[textureIdx].x, uvCoords[textureIdx].y });
				}
			}
		}

		if (vertexGroups != nullptr)
			(*vertexGroups)[currentGroupName].second = vertices->size() - 1;

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