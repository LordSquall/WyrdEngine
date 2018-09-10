#ifndef _SPRITE_SYSTEM_H_
#define _SPRITE_SYSTEM_H_

#include "export.h"

#include "subsystems\interfaces\ISubSystem.h"
#include "subsystems\interfaces\ILogger.h"
#include "subsystems\interfaces\IRenderer.h"

#include "subsystems\sprites\Sprite.h"
#include "subsystems\sprites\SpriteLayer.h"

#include <vector>
#include <map>

using namespace std;

namespace OrisisEngine
{
	class SpriteSystem : public ISubSystem {
	public:
		/* Constructors & Deconstructors */
		SpriteSystem();
		SpriteSystem(const SpriteSystem &obj);
		~SpriteSystem();

		/* ISubSystem Functions */
		void RegisterLogger(ILogger* logger);
		ILogger* GetLogger();
		
		/* Set the rendering context */
		void SetRenderer(IRenderer* renderer);
		
		/**
		* Load the required resources against the rendering context
		* @return true is resources loaded successfully
		*/
		bool LoadResources();

#pragma region File Creation functions

		/**
		* Load sprites from a json file
		* @param file - path to the json file
		* @return number of sprites successfully loaded
		*/
		int LoadSpritesFromFile(string file);

		/**
		* Load layers from a json file
		* @param file - path to the json file
		* @return number of layers successfully loaded
		*/
		int LoadLayersFromFile(string file);

		/**
		* Load texture from file and store in the sprite system
		* @param file - path to the texture file
		* @param name - unique name for the texture
		*/
		Texture* LoadTextureFromFile(string file, string name);

#pragma endregion

#pragma region Raw Creation functions

		/**
		* Create a new raw sprite
		* @param name - unique name for the sprite
		* @param width - width of the sprite in pixels
		* @param height - height of the sprite in pixels
		* @param texture - name of texture to be used for this sprite
		*/
		Sprite* CreateSprite(string name, int width, int height, string texture);

		/**
		* Create a new raw sprite layer
		* @param name - unique name for the sprite layer
		* @param texture - name of texture to be used for this spritelayer
		*/
		SpriteLayer* CreateSpriteLayer(string name, string texture);

#pragma endregion

#pragma region Getters Functions

		/**
		* Retrive a sprite by name 
		* @param name - unique name of the sprite
		*/
		Sprite* GetSprite(string name);

		/**
		* Retrive a texture by name
		* @param name - unique name of the texture
		*/
		Texture* GetTexture(string name);

		/**
		* Retrive a list of all layers
		*/
		std::map<string, SpriteLayer*> GetAllLayers();

#pragma endregion

		void Render();


	private:
		ILogger* _logger;
		IRenderer* _renderer;
		Shader	_shader;

		std::map<string, Sprite*> _sprites;

		std::map<string, Texture*> _textures;

		std::map<string, SpriteLayer*> _layers;

	};
}

#endif