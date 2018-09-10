#include "BitmapLoader.h"

#include <fstream>
#include <string>

#include "../external/bmpread/bmpread.h"

using namespace OrisisEngine;

BitmapLoader::BitmapLoader()
{
}

BitmapLoader::BitmapLoader(const BitmapLoader &obj)
{
}

BitmapLoader::~BitmapLoader()
{
}

void BitmapLoader::LoadFile(string filePath, ILogger* logger, Texture* texture)
{
	/* we begin by reading the contents of the file */
	bmpread_t bitmap;
	bmpread(filePath.c_str(), BMPREAD_ALPHA, &bitmap);
	
	/* with the file read into memory we can start populating the texture object */
	texture->Width(bitmap.width);
	texture->Height(bitmap.height);

	texture->PixelData(bitmap.data);

	bmpread_free(&bitmap);
}