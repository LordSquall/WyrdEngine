#pragma once

/* core includes */
#include <core/Log.h>

/* local includes */
#include "support/BinPacker.h"

namespace Wyrd::Editor
{
	BinPacker::BinPacker(const BinPackerConfig& config)
	{
		_Config = config;

		_Root.id = 0;
		_Root.x = 0;
		_Root.y = 0;
		_Root.width = config.size.x;
		_Root.height = config.size.y;

		_Blocks.reserve(_Config.count);
	}

	void BinPacker::Shutdown()
	{
		for (auto& block : _Blocks)
		{
			delete block;
		}
	}

	void BinPacker::AddNode(int id, int width, int height)
	{
		BinBlock* block = new BinBlock();
		block->id = id;
		block->width = width + _Config.padding.x;
		block->height = height + _Config.padding.y;

		_Blocks.push_back(block);
	}

	bool BinPacker::Pack()
	{
		/* start by calculating the area of each node */
		for (auto& node : _Blocks)
		{
			node->area = node->height * node->width;
		}

		/* sort all blocks by area */
		std::sort(_Blocks.begin(), _Blocks.end(), [](const BinBlock* a, const BinBlock* b) -> bool
			{
				return a->area > b->area;
			});

		for (auto& block : _Blocks)
		{
			BinBlock* b = FindNode(&_Root, block->width, block->height);
			if (b != nullptr)
			{
				BinBlock* sn = SplitNode(b, block->width, block->height);
				block->posX = sn->x;
				block->posY = sn->y;
			}
		}

		for (auto& block : _BlocksBuffer)
		{
			delete block;
		}

		return true;
	}


	glm::vec2 BinPacker::GetNodeIdx(int idx)
	{
		return glm::vec2(_Blocks[idx]->posX, _Blocks[idx]->posY);
	}

	glm::vec2 BinPacker::GetNodeId(int id)
	{
		auto iter = std::find_if(_Blocks.begin(), _Blocks.end(), [&id](const BinBlock* s) { return s->id == id; });
		if (iter != _Blocks.end())
		{
			return glm::vec2((*iter)->posX, (*iter)->posY);
		}
	}

	BinBlock* BinPacker::FindNode(BinBlock* node, int width, int height)
	{
		if (node->used)
		{
			BinBlock* b = FindNode(node->right, width, height);
			if (b != nullptr)
			{
				return b;
			}
			else
			{
				return FindNode(node->down, width, height);
			}
		}
		else if ((width <= node->width) && (height <= node->height))
		{
			return node;
		}
		else
		{
			return nullptr;
		}
	}

	BinBlock* BinPacker::SplitNode(BinBlock* node, int width, int height)
	{
		node->used = true;
		node->down = new BinBlock();
		node->right = new BinBlock();

		node->down->x = node->x;
		node->down->y = node->y + height;
		node->down->width = node->width;
		node->down->height = node->height - height;

		node->right->x = node->x + width;
		node->right->y = node->y;
		node->right->width = node->width - width;
		node->right->height = height;

		_BlocksBuffer.push_back(node->down);
		_BlocksBuffer.push_back(node->right);

		return node;
	}
}