/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>

/* local includes */
#include "ObjLoader.h"

/* external include */
#include <limits>

namespace Wyrd::Editor
{
	ObjLoader::Result ObjLoader::Load(std::string path, std::vector<Vertex3D>* vertices, std::vector<uint32_t>* indices, std::map<std::string, std::pair<uint32_t, uint32_t>>* vertexGroups, float scaleFactor, BoundingBox* bounds)
	{
		ObjLoader::Result result = Success;

		std::vector<Vector3> verts;
		std::vector<Vector2> uvCoords;
		std::vector<Vector3> normals;

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

			/* normals */
			if (line[0] == 'v' && line[1] == 'n')
			{
				std::string tag;
				float x, y, z;
				std::istringstream iss(line);
				iss >> tag >> x >> y >> z;

				normals.push_back({ x, y, z });
			}

			/* faces */
			if (line[0] == 'f')
			{
				char tag;
				std::string set1, set2, set3;
				std::istringstream iss(line);
				iss >> tag >> set1 >> set2 >> set3;

				{
					int vertIdx, textureIdx, normalIdx;
					sscanf(set1.c_str(), "%d/%d/%d", &vertIdx, &textureIdx, &normalIdx);
					vertIdx--;
					textureIdx--;
					normalIdx--;

					vertices->push_back({ verts[vertIdx].x, verts[vertIdx].y , verts[vertIdx].z, uvCoords[textureIdx].x, uvCoords[textureIdx].y, normals[normalIdx].x, normals[normalIdx].y , normals[normalIdx].z });
				}
				{
					int vertIdx, textureIdx, normalIdx;
					sscanf(set2.c_str(), "%d/%d/%d", &vertIdx, &textureIdx, &normalIdx);
					vertIdx--;
					textureIdx--;
					normalIdx--;

					vertices->push_back({ verts[vertIdx].x, verts[vertIdx].y , verts[vertIdx].z, uvCoords[textureIdx].x, uvCoords[textureIdx].y, normals[normalIdx].x, normals[normalIdx].y , normals[normalIdx].z });
				}
				{
					int vertIdx, textureIdx, normalIdx;
					sscanf(set3.c_str(), "%d/%d/%d", &vertIdx, &textureIdx, &normalIdx);
					vertIdx--;
					textureIdx--;
					normalIdx--;

					vertices->push_back({ verts[vertIdx].x, verts[vertIdx].y , verts[vertIdx].z, uvCoords[textureIdx].x, uvCoords[textureIdx].y, normals[normalIdx].x, normals[normalIdx].y , normals[normalIdx].z });
				}
			}
		}

		if (bounds != nullptr)
		{
			bounds->_position = { 0.0f, 0.0f, 0.0f };
			bounds->_minExtent = { FLT_MAX, FLT_MAX , FLT_MAX };
			bounds->_maxExtent = { FLT_MIN, FLT_MIN, FLT_MIN };

			for (auto& v : verts)
			{
				if (v.x > bounds->_maxExtent.x) bounds->_maxExtent.x = v.x;
				if (v.y > bounds->_maxExtent.y) bounds->_maxExtent.y = v.y;
				if (v.z > bounds->_maxExtent.z) bounds->_maxExtent.z = v.z;


				if (v.x < bounds->_minExtent.x) bounds->_minExtent.x = v.x;
				if (v.y < bounds->_minExtent.y) bounds->_minExtent.y = v.y;
				if (v.z < bounds->_minExtent.z) bounds->_minExtent.z = v.z;
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