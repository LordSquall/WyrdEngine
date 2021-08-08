#pragma once

/* core wyrd includes */
#include <core/export.h>

/* external includes */
#include <glm/glm.hpp>

namespace Wyrd::Editor
{
	struct BinPackerConfig
	{
		glm::ivec2 size;
		glm::ivec2 padding;
		unsigned int count;
	};

	struct BinBlock
	{
		int id;

		int x;
		int y;
		int width;
		int height;

		int area;
		bool used;

		int posX;
		int posY;
		BinBlock* right;
		BinBlock* down;

		BinBlock() : id(0), x(0), y(0), width(0), height(0), area(0), used(false), posX(0), posY(0), right(nullptr), down(nullptr)
		{

		}
	};

	class BinPacker
	{
	public:
		BinPacker(const BinPackerConfig& config);

		void AddNode(int id, int width, int height);

		bool Pack();

		void Shutdown();

		glm::vec2 GetNodeIdx(int idx);
		glm::vec2 GetNodeId(int id);

	private:
		BinBlock* FindNode(BinBlock* node, int width, int height);
		BinBlock* SplitNode(BinBlock* node, int width, int height);

	private:
		BinPackerConfig _Config;
		std::vector<BinBlock*> _Blocks;

		std::vector<BinBlock*> _BlocksBuffer;

		BinBlock _Root;
	};
}