#ifndef _SPRITELAYER_H_
#define _SPRITELAYER_H_

#include "export.h"

#include "subsystems\interfaces\ISubSystem.h"
#include "subsystems\interfaces\ILogger.h"
#include "subsystems\interfaces\IRenderer.h"

#include "subsystems\sprites\Sprite.h"

#include "subsystems\graphics\BatchTable.h"

using namespace std;

namespace OrisisEngine
{
	class SpriteLayer {
	public:
		SpriteLayer(string name);
		SpriteLayer(const SpriteLayer &obj);
		~SpriteLayer();

		void Initialise(IRenderer* renderer);
		void AddEntry(Sprite* sprite, int x, int y);

		void Finish();
		void Render();

		void SetTexture(Texture* texture);

		void OutputLayer(ILogger* logger);

		string Name;

	private:
		BatchTable _batchTable;
	};
}

#endif