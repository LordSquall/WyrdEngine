#pragma once

#include <Osiris.h>

using namespace Osiris;


namespace Osiris {

	class SpriteLayer;
	class SpriteBatch;
	struct SpriteBatchEntry;

	class Sprite
	{
	public:
		Sprite(std::string name, int x, int y, int width, int height) : _ID(_nextID), _Name(name), _X(x), _Y(y), _Width(width), _Height(width) 
		{
			_nextID = _nextID + 1;
		}

		~Sprite() {}

		/* Getters and Setters */
		inline const uint32_t GetID() const { return _ID; }

		inline const std::string GetName() const { return _Name; }
		inline void SetName(const std::string name) { _Name = name; }

		inline const int GetX() const { return _X; }
		void SetX(const int x);

		inline const int GetY() const { return _Y; }
		void SetY(const int y);

		inline const int GetWidth() const { return _Width; }
		void SetWidth(const int width);

		inline const int GetHeight() const { return _Height; }
		void SetHeight(const int height);

		inline const SpriteLayer* GetLayer() const { return _Layer; }
		inline void SetLayer(SpriteLayer* layer) { _Layer = layer; }

		inline const SpriteBatchEntry* GetBatchEntry() const { return _Entry; }
		inline void SetBatchEntry(SpriteBatchEntry* entry) { _Entry = entry; }

		inline const SpriteBatch* GetBatch() const { return _Batch; }
		inline void SetBatch(SpriteBatch* batch) { _Batch = batch; }

	private:
		uint32_t _ID;
		std::string _Name;
		int _X;
		int _Y;
		int _Width;
		int _Height;

		SpriteLayer* _Layer;
		SpriteBatch* _Batch;
		SpriteBatchEntry* _Entry;

		static uint32_t _nextID;

		void UpdateBatchData();
	};
}