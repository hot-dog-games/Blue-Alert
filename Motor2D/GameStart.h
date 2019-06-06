#pragma once
#include "Scene.h"
class GameStart :
	public Scene
{
public:
	GameStart();
	~GameStart();

	// Called before the first frame
	virtual bool Start();
	// Called each loop iteration
	virtual bool Update(float dt);

	// Called before quitting
	virtual bool CleanUp();
};

