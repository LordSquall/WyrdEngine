#include "Sprite.h"


using namespace OrisisEngine;

Sprite::Sprite(string name, int width, int height) : Name(name), Width(width), Height(height)
{
	this->TextureU[0] = 0.0f;
	this->TextureV[0] = 0.0f;

	this->TextureU[1] = 0.0f;
	this->TextureV[1] = 1.0f;

	this->TextureU[2] = 1.0f;
	this->TextureV[2] = 1.0f;

	this->TextureU[3] = 1.0f;
	this->TextureV[3] = 0.0f;

	_texture = NULL;
}

Sprite::Sprite(string name, int width, int height, Texture* texture) : Name(name), Width(width), Height(height), _texture(texture)
{
	this->TextureU[0] = 0.0f;
	this->TextureV[0] = 0.0f;

	this->TextureU[1] = 0.0f;
	this->TextureV[1] = 1.0f;

	this->TextureU[2] = 1.0f;
	this->TextureV[2] = 1.0f;

	this->TextureU[3] = 1.0f;
	this->TextureV[3] = 0.0f;
}


Sprite::Sprite(const Sprite &obj)
{
}

Sprite::~Sprite()
{
}

Texture* Sprite::GetTexture()
{
	return _texture;
}