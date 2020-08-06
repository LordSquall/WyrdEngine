#pragma once

/* local includes */
#include "Osiris.h"
#include "core/export.h"
#include "core/renderer/VertexArray.h"
#include "core/renderer/Buffer.h"
#include "core/pipeline/SpriteVertex.h"

using namespace Osiris;


namespace Osiris {

	class SpriteLayer;
	class SpriteBatch;
	struct SpriteBatchEntry;

	class OSR_LIBRARY_API Sprite
	{
	public:
		Sprite(std::string name, int x, int y, int width, int height);

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

		void SetPosition(const int x, const int y);
		void SetSize(const int width, const int height);

		inline const std::shared_ptr<Texture> GetTexture() const { return _Texture; }
		inline void SetTexture(std::shared_ptr<Texture> texture) { _Texture = texture; }

		inline const std::shared_ptr<VertexArray> GetVertexArray() const { return _VertexArray; }

		inline const std::shared_ptr<IndexBuffer> GetIndexBuffer() const { return _IndexBuffer; }

		inline const std::shared_ptr<VertexBuffer> GetVertexBuffer() const { return _VertexBuffer; }

	private:
		uint32_t _ID;
		std::string _Name;
		int _X;
		int _Y;
		int _Width;
		int _Height;

		std::shared_ptr<Texture> _Texture;

		std::vector<SpriteVertex> _vertices;
		std::vector<unsigned int> _indicies;
		std::shared_ptr<VertexArray> _VertexArray;
		std::shared_ptr<IndexBuffer> _IndexBuffer;
		std::shared_ptr<VertexBuffer> _VertexBuffer;

		static uint32_t _nextID;

		void UpdateBuffers();
	};
}