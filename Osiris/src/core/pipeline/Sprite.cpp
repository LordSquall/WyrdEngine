#pragma once
#include "osrpch.h"

#include "Sprite.h"

#include "SpriteBatch.h"

namespace Osiris
{
	uint32_t Sprite::_nextID = 0;

	void Sprite::SetX(const int x)
	{
		_X = x;
		UpdateBatchData();
	}

	void Sprite::SetY(const int y)
	{
		_Y = y;
		UpdateBatchData();
	}

	void Sprite::SetWidth(const int width)
	{
		_Width = width;
		UpdateBatchData();
	}

	void Sprite::SetHeight(const int height)
	{
		_Height = height;
		UpdateBatchData();
	}

	void Sprite::UpdateBatchData()
	{
		if (_Entry != NULL)
		{
			_Entry->Update(_Batch);
		}
	}
}