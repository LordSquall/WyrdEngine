#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "export.h"

#include <string>

using namespace std;

namespace OrisisEngine
{
	class Texture {
	public:
		/* Constructors & Deconstructors */
		Texture();
		Texture(const Texture &obj);
		~Texture();

		/**
		* Shutdown the object. Delete any data associated to the object
		*/
		void Shutdown();

#pragma region Getters and Setters

		/**
		* Get the assigned handle
		* @return unique handle
		*/
		unsigned int Handle();

		/**
		* Get the name
		* @return name
		*/
		string Name();

		/**
		* Get the pixel data
		* @return pointer to the start of pixel data
		*/
		unsigned char* PixelData();

		/**
		* Get the width of the texture in pixels
		* @return texture width
		*/
		int Width();

		/**
		* Get the height of the texture in pixels
		* @return texture height
		*/
		int Height();

		/**
		* Get the size of the texture in pixels
		* @return size of the texture in pixels
		*/
		int Size();

		/**
		* Get the size of the texture in bytes
		* @return size of the texture in bytes
		*/
		int SizeInBytes();

		/**
		* Set the handle for this texture
		* @param handle - handle for the texture
		*/
		void Handle(unsigned int handle);

		/**
		* Set the name for this texture
		* @param name - name for the texture
		*/
		void Name(string name);

		/**
		* Set the pixel data for this texture
		* @param data - pointer to start of pixel data
		*/
		void PixelData(unsigned char* data);

		/**
		* Set the width of  the texture
		* @param width - width of the texture
		*/
		void Width(int width);

		/**
		* Set the height of  the texture
		* @param height - height of the texture
		*/
		void Height(int height);

#pragma endregion
				
	private:
		int _handle;
		string _name;
		int _width;
		int _height;
		unsigned char* _data;
	};
}

#endif