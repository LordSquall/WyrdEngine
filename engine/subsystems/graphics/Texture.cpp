#include "Texture.h"

#include <malloc.h>
#include <memory.h>

using namespace OrisisEngine;

Texture::Texture()
{
	_handle = 0;
	_width = 0;
	_height = 0;
	_data = 0;
}

Texture::Texture(const Texture &obj)
{
}

Texture::~Texture()
{
}

void Texture::Shutdown()
{
	if (this->_data != nullptr)
	{
		delete this->_data;
	}
}

#pragma region Getters and Setters

unsigned int Texture::Handle()
{
	return _handle;
}

string Texture::Name()
{
	return _name;
}

unsigned char* Texture::PixelData()
{
	return _data;
}

int Texture::Width()
{
	return _width;
}

int Texture::Height()
{
	return _height;
}

int Texture::Size()
{
	return _width * _height;
}

int Texture::SizeInBytes()
{
	return _width * _height * 4;
}

void Texture::Handle(unsigned int handle)
{
	_handle = handle;
}

void Texture::Name(string name)
{
	_name.assign(name);
}

void Texture::PixelData(unsigned char* data)
{
	/* we need to check to make sure that we have allocated memory for this texture */
	if (_data == 0)
	{
		_data = (unsigned char*)malloc(sizeof(unsigned char) * SizeInBytes());
	}

	/* copy of the data into the texture */
	memcpy(_data, data, sizeof(unsigned char) * SizeInBytes());
}

void Texture::Width(int width)
{
	_width = width;
}

void Texture::Height(int height)
{
	_height = height;
}

#pragma endregion