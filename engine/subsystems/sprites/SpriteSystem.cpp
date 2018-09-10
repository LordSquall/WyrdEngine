#include "SpriteSystem.h"

#include "subsystems\loaders\SpriteLoader.h"
#include "subsystems\loaders\SpriteLayerLoader.h"
#include "subsystems\loaders\BitmapLoader.h"
#include "subsystems\loaders\ShaderLoader.h"

#include <iterator>


using namespace OrisisEngine;

SpriteSystem::SpriteSystem()
{
}

SpriteSystem::SpriteSystem(const SpriteSystem &obj)
{
}

SpriteSystem::~SpriteSystem()
{
	for (std::map<string, SpriteLayer*>::iterator it = _layers.begin(); it != _layers.end(); ++it)
	{
		delete it->second;
	}

	for (std::map<string, Texture*>::iterator it = _textures.begin(); it != _textures.end(); ++it)
	{
		it->second->Shutdown();
		delete it->second;
	}

	for (std::map<string, Sprite*>::iterator it = _sprites.begin(); it != _sprites.end(); ++it)
	{
		delete it->second;
	}
}

void SpriteSystem::RegisterLogger(ILogger* logger)
{
	_logger = logger;
}

ILogger* SpriteSystem::GetLogger()
{
	return _logger;
}

void SpriteSystem::SetRenderer(IRenderer* renderer)
{
	_renderer = renderer;
}

bool SpriteSystem::LoadResources()
{
	if (ShaderLoader::LoadFile("../engine/resources/sprite.vert", "../engine/resources/sprite.frag", _logger, &_shader))
	{
		_renderer->RegisterShader(&_shader);
		return true;
	}

	return false;
}

#pragma region File Creation functions

Texture* SpriteSystem::LoadTextureFromFile(string file, string name)
{
	/* we can start by creating a new texture within the sprite system */
	Texture* newTexture = new Texture();
	newTexture->Name(name);
	_textures[name] = newTexture;

	/* next we need to populate the texture with data from the file */
	BitmapLoader::LoadFile(file, _logger, newTexture);

	/* we can create the texture on the renderer */
	_renderer->CreateTexture(newTexture);

	/* finally we can push the pixel data from the CPU to the renderer */
	_renderer->UpdateTexture(newTexture);

	return newTexture;
}

int SpriteSystem::LoadSpritesFromFile(string file)
{
	/* load the sprite data from the support configuration file */
	return SpriteLoader::LoadFile(file, _logger, this);
}


int SpriteSystem::LoadLayersFromFile(string file)
{
	return SpriteLayerLoader::LoadFile(file, _logger, this);
}

#pragma endregion

#pragma region Raw Creation functions


Sprite* SpriteSystem::CreateSprite(string name, int width, int height, string texture)
{
	Texture* t = GetTexture(texture);

	Sprite* newSprite = new Sprite(name, width, height, t);

	this->_sprites[name] = newSprite;

	return newSprite;
}

SpriteLayer* SpriteSystem::CreateSpriteLayer(string name, string texture)
{
	/* we need to find the texture altas for this layer */
	Texture* layerTexture = GetTexture(texture);

	SpriteLayer* newLayer = new SpriteLayer(name);
	newLayer->Initialise(_renderer);
	newLayer->SetTexture(layerTexture);

	this->_layers[name] = newLayer;

	return newLayer;
}

#pragma endregion

#pragma region Getters Functions

Sprite* SpriteSystem::GetSprite(string name)
{
	return this->_sprites[name];
}

Texture* SpriteSystem::GetTexture(string name)
{
	return this->_textures[name];
}

std::map<string, SpriteLayer*> SpriteSystem::GetAllLayers()
{
	return this->_layers;
}

#pragma endregion

void SpriteSystem::Render()
{
	for (std::map<string, SpriteLayer*>::iterator it = _layers.begin(); it != _layers.end(); ++it)
	{
		it->second->Render();
	}
}