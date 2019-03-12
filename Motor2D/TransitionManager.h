#ifndef _TRANSITION_MANAGER_H_
#define _TRANSITION_MANAGER_H_

//This is the module that Marc will do on his research?

#include "Module.h"

class TransitionManager :
	public Module
{
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

};

#endif
