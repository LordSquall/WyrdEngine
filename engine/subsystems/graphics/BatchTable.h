#ifndef _BATCHTABLE_H_
#define _BATCHTABLE_H_

#include "export.h"

#include <vector>

#include "subsystems\interfaces\IRenderer.h"

#include "subsystems\sprites\Sprite.h"

#include "subsystems\graphics\VertexArray.h"
#include "subsystems\graphics\VertexBuffer.h"

using namespace std;

namespace OrisisEngine
{
	struct BatchTable_Entry {
	
		unsigned int offset;

		BatchTable_Entry(unsigned int offset) : offset(offset) { }
	};

	class BatchTable {
	public:
		BatchTable();
		BatchTable(const BatchTable &obj);
		~BatchTable();

		void Initialise(IRenderer* renderer);

		void AddSpriteEntry(Sprite* sprite, int x, int y);

		void PushAllEntries();

		void Render();

		vector<BatchTable_Entry> GetAllEntry();

	public:
		Texture*		 TextureAtlas;

	private:
		IRenderer* _renderer;
		vector<BatchTable_Entry> _entries;
		VertexArray  _vertexArray;
		VertexBuffer _vertexBuffer;
	};
}

#endif