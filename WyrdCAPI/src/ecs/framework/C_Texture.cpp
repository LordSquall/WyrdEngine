#include "C_Texture.h" 
#include "core/Resources.h"
#include "core/renderer/Texture.h"


INT32 Texture_GetWidth(void* nativePtr)
{
	Wyrd::Texture* texture = (Wyrd::Texture*)nativePtr;
	return texture->GetWidth();
}

INT32 Texture_GetHeight(void* nativePtr)
{
	Wyrd::Texture* texture = (Wyrd::Texture*)nativePtr;
	return texture->GetHeight();
}
