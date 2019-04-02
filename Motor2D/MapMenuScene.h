#ifndef _MAP_MENU_SCENE_H_
#define _MAP_MENU_SCENE_H_

#include "Scene.h"
#include "p2Point.h"

struct SDL_Texture;
struct UIButton;
struct UIImage;
class Entity;
class Summoner;
class UIBar;

struct Card;

class MapMenuScene :
	public Scene
{
public:
	MapMenuScene();
	~MapMenuScene();

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

	virtual bool GUIEvent(UIElement* element, GUI_Event gui_event);

private:
	SDL_Texture*	debug_tex;
	SDL_Texture*	ui_background;


	Entity* test_core;
	Summoner* test_summoner;

	UIImage* current_drag;
};

#endif
