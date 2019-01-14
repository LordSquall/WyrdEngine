#pragma once

namespace Osiris {

	class Vertex
	{
	public:
		Vertex() : x(0.0f), y(0.0f), z(0.0f) {};
		Vertex(float x, float y, float z) : x(x), y(y), z(z) { }
		float x, y, z;
	};
}