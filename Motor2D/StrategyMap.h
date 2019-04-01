#ifndef _STRATEGY_MAP_H_
#define _STRATEGY_MAP_H_

#include "Scene.h"
#include "p2Point.h"
#include "EncounterTree.h"


struct SDL_Texture;

class StrategyMap :
	public Scene
{
public:
	StrategyMap();
	~StrategyMap();

	// Called before the first frame
	virtual bool Start();

	// Called before all Updates
	virtual bool PreUpdate();

	// Called each loop iteration
	virtual bool Update(float dt);

	// Called before all Updates
	virtual bool PostUpdate();

	// Called before quitting
	virtual bool CleanUp();

private:
	SDL_Texture * debug_tex;
	EncounterTree* level = nullptr;
};

#endif

