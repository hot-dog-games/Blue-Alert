#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "Render.h"
#include "Textures.h"

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

Textures::Textures() : Module()
{
	name = "textures";
}

// Destructor
Textures::~Textures()
{}

// Called before render is available
bool Textures::Awake(pugi::xml_node& config)
{
	LOG("Init Image library");
	bool ret = true;
	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool Textures::Start()
{
	LOG("start textures");
	bool ret = true;
	return ret;
}

// Called before quitting
bool Textures::CleanUp()
{
	LOG("Freeing textures and Image library");
	std::map<std::string, SDL_Texture*>::iterator item;

	for(item = textures.begin(); item != textures.end(); ++item)
	{
		SDL_DestroyTexture(item->second);
		textures.erase(item);
	}

	textures.clear();
	IMG_Quit();
	return true;
}

// Load new texture from file path
SDL_Texture* const Textures::Load(const char* path)
{
	SDL_Texture* texture = NULL;

	std::map<std::string, SDL_Texture*>::iterator item = textures.find(path);
	
	if (item == textures.end())
	{
		SDL_Surface* surface = IMG_Load(path);

		if (surface == NULL)
		{
			LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
		}
		else
		{
			texture = LoadSurface(surface, path);
			SDL_FreeSurface(surface);
		}
	}
	else
		texture = item->second;

	return texture;
}

// Unload texture by SDL_Texture
bool Textures::UnLoad(SDL_Texture* texture)
{
	std::map<std::string, SDL_Texture*>::iterator item;

	for(item = textures.begin(); item != textures.end(); ++item)
	{
		if(texture == item->second)
		{
			SDL_DestroyTexture(item->second);
			textures.erase(item);
			return true;
		}
	}

	return false;
}

// Unload texture by path
bool Textures::UnLoad(std::string path)
{
	std::map<std::string, SDL_Texture*>::iterator item = textures.find(path);

	if (item != textures.end())
	{
		SDL_DestroyTexture(item->second);
		textures.erase(item);
		return true;
	}

	return false;
}

// Translate a surface into a texture
SDL_Texture* const Textures::LoadSurface(SDL_Surface* surface, const char* path)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(App->render->renderer, surface);

	if(texture == NULL)
	{
		LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		textures.insert({path,texture});
	}

	return texture;
}

// Retrieve size of a texture
void Textures::GetSize(const SDL_Texture* texture, uint& width, uint& height) const
{
	SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, (int*) &width, (int*) &height);
}
