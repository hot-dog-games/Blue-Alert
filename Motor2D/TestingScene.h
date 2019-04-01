#ifndef _TESTING_SCENE_H_
#define _TESTING_SCENE_H_

#include "Scene.h"
#include "p2Point.h"
#include "EncounterTree.h"

struct SDL_Texture;
class Core;

struct Card;

class TestingScene :
	public Scene
{
public:
	TestingScene();
	~TestingScene();

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
	SDL_Texture* debug_tex;
	Core* test_core;
	EncounterTree* level = nullptr;
};

#endif
