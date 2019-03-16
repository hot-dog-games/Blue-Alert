#ifndef _TRANSITION_MANAGER_H_
#define _TRANSITION_MANAGER_H_

//This is the module that Marc will do on his research?

#include "Module.h"
#include "Transition.h"
#include "Color.h"

class TransitionManager :
	public Module
{
public:


private:
	std::list<Transition*> active_transitions;

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

	void CreateTransition(Transition::TransitionType type, float transition_time, bool is_scene_change = false, int scene_to_transition = 0);
	void CreateFadeTransition(float transition_time, bool is_scene_change = false, int scene_to_transition = 0, Color color = {0, 0, 0, 255});
	void CreateZoomTransition(float transition_time, bool is_scene_change = false, int scene_to_transition = 0);
	void DestroyTransition(Transition* transition_to_destroy);
};

#endif
