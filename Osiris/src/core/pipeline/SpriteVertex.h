#pragma once

#include <Osiris.h>

namespace Osiris {
	/* represents a single sprite vertex */
	struct SpriteVertex
	{
		float x, y;		/* 2D position		*/
		float u, v;		/* texture coords	*/
	};
}