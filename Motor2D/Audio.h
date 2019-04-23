#ifndef __Audio_H__
#define __Audio_H__

#include "Module.h"
#include <map>
#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

class Audio : public Module
{
public:

	Audio();

	// Destructor
	virtual ~Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	const char* LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(const char* fx, int repeat = 0);

	void SetFXVolume(const char* path, int volume);

private:

	_Mix_Music*	music = NULL;
	std::map<std::string, Mix_Chunk*>	fx;
};

#endif // __Audio_H__
