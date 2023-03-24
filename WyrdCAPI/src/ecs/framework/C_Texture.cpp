#include "C_Texture.h" 
#include "core/Resources.h"
#include "core/renderer/Texture.h"


int32_t Texture_GetWidth(void* nativePtr)
{
	Wyrd::Texture* texture = (Wyrd::Texture*)nativePtr;
	return texture->GetWidth();
}

int32_t Texture_GetHeight(void* nativePtr)
{
	Wyrd::Texture* texture = (Wyrd::Texture*)nativePtr;
	return texture->GetHeight();
}
