#pragma once

#include <Osiris.h>

using namespace Osiris;


namespace Osiris {

	class Sprite
	{
	public:
		Sprite(int x, int y, int width, int height) : _X(x), _Y(y), _Width(width), _Height(width) {}

		~Sprite() {}

		/* Getters and Setters */
		inline const int GetX() const { return _X; }
		inline void SetX(const int x) { _X = x; }

		inline const int GetY() const { return _Y; }
		inline void SetY(const int y) { _Y = y; }

		inline const int GetWidth() const { return _Width; }
		inline void SetWidth(const int width) { _Width = width; }

		inline const int GetHeight() const { return _Height; }
		inline void SetHeight(const int height) { _Height = height; }

	private:
		int _X;
		int _Y;
		int _Width;
		int _Height;
	};
}