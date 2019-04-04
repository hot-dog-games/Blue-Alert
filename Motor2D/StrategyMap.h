#ifndef _STRATEGY_MAP_H_
#define _STRATEGY_MAP_H_

#include "Scene.h"
#include "p2Point.h"
#include "EncounterTree.h"
#include "UIButton.h"
#include "UIImage.h"


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

	virtual bool GUIEvent(UIElement* element, GUI_Event gui_event);

	void HideMenu();

private:

	EncounterTree* encounter_tree = nullptr;
	EncounterNode* current_node = nullptr;

	SDL_Texture*	background;


	UIImage* banner;
	UIImage* options;
	UILabel* health;
	UILabel* gold;
	UILabel* energy;
	UIButton* settings_button;
	UIButton* menu_button;
	UIImage* main_panel;
};

#endif

