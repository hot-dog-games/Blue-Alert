#ifndef __Textures_H__
#define __Textures_H__

#include <map>
#include "Module.h"

struct SDL_Texture;
struct SDL_Surface;

class Textures : public Module
{
public:

	Textures();

	// Destructor
	virtual ~Textures();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before quitting
	bool CleanUp();

	// Load Texture
	SDL_Texture* const	Load(const char* path);
	bool				UnLoad(SDL_Texture* texture);
	bool				UnLoad(std::string path);
	SDL_Texture* const	LoadSurface(SDL_Surface* surface, const char* path = nullptr);
	void				GetSize(const SDL_Texture* texture, uint& width, uint& height) const;

public:
	std::map<std::string, SDL_Texture*>	textures;
};


#endif // __Textures_H__
