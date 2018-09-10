#include "SpriteLayer.h"

using namespace OrisisEngine;

SpriteLayer::SpriteLayer(string name) :Name(name)
{
}

SpriteLayer::SpriteLayer(const SpriteLayer &obj)
{
}

SpriteLayer::~SpriteLayer()
{
}

void SpriteLayer::Initialise(IRenderer* renderer)
{
	/* initialise the batch table */
	_batchTable.Initialise(renderer);
}

void SpriteLayer::AddEntry(Sprite* sprite, int x, int y)
{
	_batchTable.AddSpriteEntry(sprite, x, y);
}

void SpriteLayer::Finish()
{
	_batchTable.PushAllEntries();
}

void SpriteLayer::Render()
{
	_batchTable.Render();
}

void SpriteLayer::SetTexture(Texture* texture)
{
	_batchTable.TextureAtlas = texture;
}


void SpriteLayer::OutputLayer(ILogger* logger)
{
	for (BatchTable_Entry entry : _batchTable.GetAllEntry())
	{
		logger->LogDebug("Entry: " + to_string(entry.offset));
	}
}