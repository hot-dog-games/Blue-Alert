#ifndef _TRANSITION_MANAGER_H_
#define _TRANSITION_MANAGER_H_

//This is the module that Marc will do on his research?

#include "Module.h"
#include "Timer.h"
#include "p2Point.h"
#include "SDL\include\SDL_rect.h"

class TransitionManager :
	public Module
{

	enum class TransitionType {
		FADE,
		ZOOM
	};

	enum class TransitionState {
		NONE,
		ENTERING,
		EXITING
	};

private:
	TransitionType type;
	TransitionState state;

	float transition_time;
	Timer* current_time = nullptr;

	int scene_to_transition;

	//Util variables for diverse transitions
	SDL_Rect screen;


public:
	TransitionManager();
	~TransitionManager();

	// Called before render is available
	virtual bool Awake(pugi::xml_node&);
	// Called before the first frame
	virtual bool Start();
	// Called each loop iteration
	virtual bool PreUpdate();
	// Called each loop iteration
	virtual bool Update(float dt);
	// Called each loop iteration
	virtual bool PostUpdate();
	// Called before quitting
	virtual bool CleanUp();

	//--------------------------------

	void ChangeSceneWithFade(Timer time);
	void FadeIn();
	void FadeOut();

	void Zoom(iPoint position, uint quantity = 4);

};

#endif
